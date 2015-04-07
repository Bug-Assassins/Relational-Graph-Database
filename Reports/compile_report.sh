#!bin/bash

#Depends on Inkscape and PDFLatex

#Converting Images to Pdf
if hash inkscape > /dev/null 2>&1;
then
    #using inkscape

    inkscape -D -z --file=pics/foreign_key.svg --export-pdf=pics/foreign_key.pdf

    inkscape -D -z --file=pics/model.svg --export-pdf=pics/model.pdf

    inkscape -D -z --file=pics/primary_key.svg --export-pdf=pics/primary_key.pdf

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
fi

#Compiling Latex which includes pdfs
if ! hash pdflatex > /dev/null 2>&1;
then
    apt-get install pdflatex
fi

pdflatex final_report

#Cleaning up junk files generated
rm final_report.aux
rm final_report.lof
rm final_report.log
rm final_report.lot
rm final_report.out
rm final_report.tex.backup
rm final_report.toc
rm pics/*.pdf
