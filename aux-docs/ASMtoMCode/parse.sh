touch test.asm MCode.mc
echo "$*" > test.asm
g++ A-to-M.cpp -w -o A && ./A
str=$(head -n 1 MCode.mc)
v=$(echo $str | cut -d' ' -f2)
echo ${v:8:2} ${v:6:2} ${v:4:2} ${v:2:2}
echo " "
rm A test.asm MCode.mc
