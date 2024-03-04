echo on
cd /D %~dp0
del html\flags.html
del html\*.png
call asciidoctor -r asciidoctor-diagram -b html5 -o html\flags.html index.adoc
rem copy .\flags.html .\html\flags.html
rem copy *.png .\html
pause