#!/usr/bin/env python3
import sys
from pypdf import PdfReader, PdfWriter

if len(sys.argv) != 3:
    print("Usage: test_script.py <input_pdf> <output_pdf>")
    sys.exit(1)

input_pdf = sys.argv[1]
output_pdf = sys.argv[2]

reader = PdfReader(input_pdf)
writer = PdfWriter()

for page in reader.pages:
    writer.add_page(page)
    writer.add_page(page)

with open(output_pdf, "wb") as f:
    writer.write(f)

print(f"Duplicated pages and saved to {output_pdf}")
