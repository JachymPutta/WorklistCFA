#!/bin/bash

./main | dot -Tpdf | csplit --quiet --elide-empty-files --prefix=tmpforestfile - "/%%EOF/+1" "{*}" && pdftk tmpforestfile* cat output forest.pdf && rm -f tmpforestfile*
