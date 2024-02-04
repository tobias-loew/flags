echo on
cd /D %~dp0
call asciidoctor -b html5 -o flags.html index.adoc
copy .\flags.html .\html\flags.html
pause