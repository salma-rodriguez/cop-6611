DOC = paper

all: 
	pdflatex $(DOC)
	bibtex $(DOC)
	pdflatex $(DOC)
	pdflatex $(DOC)

clean:
	-rm -f *.pdf *.aux *.log *.ps *.dvi *.bbl *.blg

.PHONY: all clean
