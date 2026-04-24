#pragma once

#include "BaseExportJob.h"
#include "filesystem.h"

class Control;

class PdfExportViaPythonJob: public BaseExportJob {
public:
    PdfExportViaPythonJob(Control* control);

protected:
    ~PdfExportViaPythonJob() override;

public:
    void run() override;

protected:
    void addFilterToDialog(GtkFileChooser* dialog) override;
    void setExtensionFromFilter(fs::path& p, const char* filterName) const override;
};
