#include "PdfExportViaPythonJob.h"

#include <memory>
#include <string>
#include <utility>
#include <cstdlib>

#include "control/Control.h"
#include "control/jobs/BaseExportJob.h"
#include "model/Document.h"
#include "pdf/base/XojPdfExport.h"
#include "pdf/base/XojPdfExportFactory.h"
#include "util/PathUtil.h"
#include "util/i18n.h"

PdfExportViaPythonJob::PdfExportViaPythonJob(Control* control): BaseExportJob(control, _("PDF Export via Python")) {}

PdfExportViaPythonJob::~PdfExportViaPythonJob() = default;

void PdfExportViaPythonJob::addFilterToDialog(GtkFileChooser* dialog) {
    addFileFilterToDialog(dialog, _("PDF files"), "application/pdf");
}

void PdfExportViaPythonJob::setExtensionFromFilter(fs::path& file, const char* /*filterName*/) const {
    Util::clearExtensions(file, ".pdf");
    file += ".pdf";
}

void PdfExportViaPythonJob::run() {
    Document* doc = control->getDocument();

    doc->lock_shared();
    std::unique_ptr<XojPdfExport> pdfe = XojPdfExportFactory::createExport(doc, control);
    doc->unlock_shared();

    fs::path tempFile = fs::temp_directory_path() / "xournalpp_temp_export.pdf";

    if (!pdfe->createPdf(tempFile, false)) {
        this->errorMsg = pdfe->getLastError();
        if (control->getWindow()) {
            callAfterRun();
        }
        return;
    }

    fs::path scriptPath = Util::getConfigSubfolder() / "xournalpp_export_script.py";
    std::string command = "python3 \"" + scriptPath.string() + "\" \"" + tempFile.string() + "\" \"" + this->filepath.string() + "\"";
    
    int ret = system(command.c_str());
    if (ret != 0) {
        this->errorMsg = "Python script failed with code " + std::to_string(ret);
        if (control->getWindow()) {
            callAfterRun();
        }
    }
}
