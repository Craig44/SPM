import string
import re

p = re.compile('\{.*?\}',re.IGNORECASE)
q = re.compile('\}.*?\{',re.IGNORECASE)
r = re.compile('[label]',re.IGNORECASE)

FILE = [open('Syntax/PopulationSyntax.tex', 'r'),
        open('Syntax/EstimationSyntax.tex', 'r'),
        open('Syntax/ObservationSyntax.tex', 'r'),
        open('Syntax/ReportSyntax.tex', 'r'),
        open('Syntax/GeneralSyntax.tex', 'r')]
SYNTAX_OUTFILE = open('SPM.xml', 'w')
VERSION = open('SPMversion.tex', 'r')

SYNTAX_OUTFILE.write("<!--Notepad++ syntax definitions for SPM version ")
SYNTAX_OUTFILE.write(VERSION.read())
SYNTAX_OUTFILE.write("    ========================================================-->\n\n")
SYNTAX_OUTFILE.write("<NotepadPlus>\n")
SYNTAX_OUTFILE.write("    <UserLang name=\"SPM\" ext=\"spm\" udlVersion=\"2.1\">\n")
SYNTAX_OUTFILE.write("        <Settings>\n")
SYNTAX_OUTFILE.write("            <Global caseIgnored=\"yes\" allowFoldOfComments=\"no\" foldCompact=\"no\" forcePureLC=\"0\" decimalSeparator=\"0\" />\n")
SYNTAX_OUTFILE.write("            <Prefix Keywords1=\"no\" Keywords2=\"no\" Keywords3=\"no\" Keywords4=\"no\" Keywords5=\"no\" Keywords6=\"no\" Keywords7=\"no\" Keywords8=\"no\" />\n")
SYNTAX_OUTFILE.write("        </Settings>\n")
SYNTAX_OUTFILE.write("        <KeywordLists>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Comments\">00# 01 02 03{ 04}</Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, prefix1\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, prefix2\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, extras1\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, extras2\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, suffix1\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, suffix2\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Numbers, range\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Operators1\">-+*/!~%^&amp;|=$:!_&lt;&gt;</Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Operators2\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code1, open\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code1, middle\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code1, close\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code2, open\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code2, middle\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in code2, close\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in comment, open\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in comment, middle\"></Keywords>\n")
SYNTAX_OUTFILE.write("            <Keywords name=\"Folders in comment, close\"></Keywords>\n")

Keywords1 = []
Keywords2 = []
Keywords3 = ["F","False","T","True","t","f","true","false"]
Keywords4 = []
Keywords5 = []
Keywords6 = []

for i in range(len(FILE)):
  count=0
  for line in FILE[i]:
    # strip white space at start & end
    line = line.strip()
    # if a line has a label, then delete it
    find = line.find('\label')
    # if line if not blank, add a closing bracket
    if line != "":
      line = line[0:find] + "}"
    # recode subsections, subsubsection to be excluded from the contents
    if line[1:14] == "subsubsection" :
      mat = line.find(']')+1
      line = "\\par\\textbf" + line[mat:] + "\\par"
    if line[1:11] == "subsection" :
      #line = "\\subsubsection" + line[11:]
      line = ""
    if line[1:8] == "section" :
      line = "\\subsection" + line[8:]
    # keep line only if it is a section, defCom, defComlab, defComArg, or defSub
    if (line[1:11]=="subsection" or line[1:14]=="subsubsection" or line[1:11]=="par\\textbf" or line[1:7]=="defCom" or line[1:7]=="defSub") :
      if (line[1:11]=="subsection") :
        count=0
        line = line + "\\par"
      if (line[1:7]=="defCom") :
        m = p.search(line)
        Keywords1.append(m.group()[1:(len(m.group())-1)])
        line = line + "\\par"
        if (count!=0):
          line = "\\par" + line
        else:
          count=1
        if(i > 0):
          line = line + "\\par"
      if (line[1:7]=="defSub") :
        m = p.search(line)
        Keywords2.append(m.group()[1:(len(m.group())-1)])
      if (line[1:11]=="par\\textbf") :
        m = q.search(line[::-1])
        n = m.group()[::-1]
        Keywords4.append(n[1:(len(n)-2)])

# Remove duplicates
Keywords1 = list(set(Keywords1))
Keywords2 = list(set(Keywords2))
Keywords3 = list(set(Keywords3))
Keywords4 = list(set(Keywords4))
Keywords5 = list(set(Keywords5))
Keywords6 = list(set(Keywords6))
# Sort
Keywords1.sort()
Keywords2.sort()
Keywords3.sort()
Keywords4.sort()
Keywords5.sort()
Keywords6.sort()
# remove subscript [label]

#Remove '\' in each string
for i in range(len(Keywords1)):
  Keywords1[i] = Keywords1[i].replace("\\", "")
for i in range(len(Keywords2)):
  Keywords2[i] = Keywords2[i].replace("\\", "")
  if(Keywords2[i].count('[label]')):
    Keywords2[i]=Keywords2[i][0:(Keywords2[i].find('[label]')-1)]
for i in range(len(Keywords3)):
  Keywords3[i] = Keywords3[i].replace("\\", "")
for i in range(len(Keywords4)):
  Keywords4[i] = Keywords4[i].replace("\\", "")
for i in range(len(Keywords5)):
  Keywords5[i] = Keywords5[i].replace("\\", "")
for i in range(len(Keywords6)):
  Keywords6[i] = Keywords6[i].replace("\\", "")

SYNTAX_OUTFILE.write("            <Keywords name=\"Keywords1\">")
for i in Keywords1 :
  SYNTAX_OUTFILE.write("@" + i)
  SYNTAX_OUTFILE.write("&#x000D;&#x000A;")
SYNTAX_OUTFILE.write("</Keywords>")

SYNTAX_OUTFILE.write("\n            <Keywords name=\"Keywords2\">")
for i in Keywords2 :
  SYNTAX_OUTFILE.write(i)
  SYNTAX_OUTFILE.write("&#x000D;&#x000A;")
SYNTAX_OUTFILE.write("</Keywords>")

SYNTAX_OUTFILE.write("\n            <Keywords name=\"Keywords3\">")
for i in Keywords3 :
  SYNTAX_OUTFILE.write(i)
  SYNTAX_OUTFILE.write("&#x000D;&#x000A;")
SYNTAX_OUTFILE.write("</Keywords>")

SYNTAX_OUTFILE.write("\n            <Keywords name=\"Keywords4\">")
for i in Keywords4 :
  SYNTAX_OUTFILE.write(i)
  SYNTAX_OUTFILE.write("&#x000D;&#x000A;")
SYNTAX_OUTFILE.write("</Keywords>")

SYNTAX_OUTFILE.write('\n')
SYNTAX_OUTFILE.write('            <Keywords name="Keywords5"></Keywords>\n')
SYNTAX_OUTFILE.write('            <Keywords name="Keywords6"></Keywords>\n')
SYNTAX_OUTFILE.write('            <Keywords name="Keywords7"></Keywords>\n')
SYNTAX_OUTFILE.write('            <Keywords name="Keywords8"></Keywords>\n')
SYNTAX_OUTFILE.write('            <Keywords name="Delimiters">00&quot; 01 02&quot; 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19 20 21 22 23</Keywords>\n')
SYNTAX_OUTFILE.write('        </KeywordLists>\n')
SYNTAX_OUTFILE.write('        <Styles>\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DEFAULT" fgColor="000000" bgColor="FFFFFF" fontName="&#x6180;&#xC305;&#x0273;" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="COMMENTS" fgColor="808040" bgColor="FFFFFF" fontName="0" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="LINE COMMENTS" fgColor="808040" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="NUMBERS" fgColor="000000" bgColor="FFFFFF" fontName="0" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS1" fgColor="FF0000" bgColor="FFFFFF" fontName="4" fontStyle="1" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS2" fgColor="0000FF" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS3" fgColor="800000" bgColor="FFFFFF" fontName="Backup" fontStyle="1" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS4" fgColor="8080FF" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS5" fgColor="000000" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS6" fgColor="000000" bgColor="FFFFFF" fontName="MRU" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS7" fgColor="000000" bgColor="FFFFFF" fontName="&#x7020;&#xC305;&#x0273;" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="KEYWORDS8" fgColor="000000" bgColor="FFFFFF" fontName="no" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="OPERATORS" fgColor="000000" bgColor="FFFFFF" fontName="no" fontStyle="1" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="FOLDER IN CODE1" fgColor="000000" bgColor="FFFFFF" fontName="&#x9BD0;&#xC312;&#x0273;" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="FOLDER IN CODE2" fgColor="000000" bgColor="FFFFFF" fontName="&#x6D80;&#xC305;&#x0273;" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="FOLDER IN COMMENT" fgColor="000000" bgColor="FFFFFF" fontName="Caret" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS1" fgColor="804000" bgColor="FFFFFF" fontName="no" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS2" fgColor="000000" bgColor="FFFFFF" fontName="&#x6FF0;&#xC305;&#x0273;" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS3" fgColor="000000" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS4" fgColor="000000" bgColor="FFFFFF" fontName="no" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS5" fgColor="000000" bgColor="FFFFFF" fontName="box" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS6" fgColor="000000" bgColor="FFFFFF" fontName="no" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS7" fgColor="000000" bgColor="FFFFFF" fontName="2" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('            <WordsStyle name="DELIMITERS8" fgColor="000000" bgColor="FFFFFF" fontName="" fontStyle="0" nesting="0" />\n')
SYNTAX_OUTFILE.write('        </Styles>\n')
SYNTAX_OUTFILE.write('    </UserLang>\n')
SYNTAX_OUTFILE.write('</NotepadPlus>\n')

