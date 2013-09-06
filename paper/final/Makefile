AUX_FILES = report.aux report.bbl report.blg report.log

report.pdf: report.tex report.bib multiqueue.png two-lock.png
	pdflatex report.tex
	bibtex report.aux
	pdflatex report.tex
	pdflatex report.tex

.PHONY: clean
clean:
	-$(RM) $(AUX_FILES)
