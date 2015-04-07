#!bin/bash

#Depends on Inkscape and PDFLatex

#Converting Images to Pdf
if hash inkscape > /dev/null 2>&1;
then
    #using inkscape

    inkscape -D -z --file=pics/foreign_key.svg --export-pdf=pics/foreign_key.pdf

    inkscape -D -z --file=pics/model.svg --export-pdf=pics/model.pdf

    inkscape -D -z --file=pics/primary_key.svg --export-pdf=pics/primary_key.pdf

    inkscape -D -z --file=pics/trie.svg --export-pdf=pics/trie.pdf

    inkscape -D -z --file=pics/100.svg --export-pdf=pics/100.pdf

    inkscape -D -z --file=pics/1000.svg --export-pdf=pics/1000.pdf

    inkscape -D -z --file=pics/10000.svg --export-pdf=pics/10000.pdf

    inkscape -D -z --file=pics/90000.svg --export-pdf=pics/90000.pdf

    inkscape -D -z --file=pics/time.svg --export-pdf=pics/time.pdf

else
    if ! hash rsvg-convert > /dev/null 2>&1;
    then
        #Install Requird Library
        apt-get install librsvg2-bin
    fi
    #Use rsvg

    rsvg-convert -f pdf -o pics/model.pdf pics/model.svg

    rsvg-convert -f pdf -o pics/foreign_key.pdf pics/foreign_key.svg

    rsvg-convert -f pdf -o pics/primary_key.pdf pics/primary_key.svg

    rsvg-convert -f pdf -o pics/trie.pdf pics/trie.svg

    rsvg-convert -f pdf -o pics/100.pdf pics/100.svg

    rsvg-convert -f pdf -o pics/1000.pdf pics/1000.svg

    rsvg-convert -f pdf -o pics/10000.pdf pics/10000.svg

    rsvg-convert -f pdf -o pics/90000.pdf pics/90000.svg

    rsvg-convert -f pdf -o pics/time.pdf pics/time.svg

fi

#Compiling Latex which includes pdfs
if ! hash pdflatex > /dev/null 2>&1;
then
    apt-get install pdflatex
fi

pdflatex final_report.tex
pdflatex final_presentation.tex

#Cleaning up junk files generated
rm *.aux
rm *.lof
rm *.log
rm *.lot
rm *.out
rm *.tex.backup
rm *.toc
rm *.nav
rm *.snm
