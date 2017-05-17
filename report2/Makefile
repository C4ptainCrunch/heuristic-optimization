all: main.pdf


main.pdf: main.tex
	pdflatex main.tex
	bibtex main
	pdflatex main.tex
	pdflatex main.tex

clean:
	rm -rf main.aux  main.log  main.out  main.pdf  main.bbl  main.blg
