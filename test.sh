#!/bin/sh
rm -fv Test/program
cp $1 Test/program

cd "Test/Area";
program="../program"
area='./'

t1="../T1";
t2="../T2";
t3="../T3";

s1=$(ls $t1/in/*.in | cut -d'/' -f 4| cut -d'.' -f 1);
s2=$(ls $t2/in/*.in | cut -d'/' -f 4| cut -d'.' -f 1);
s3=$(ls $t3/in/*.in | cut -d'/' -f 4| cut -d'.' -f 1);

# Clear

# T1
cp $program program;
cp $t1/*.bin $area;
cp $t1/*.csv $area;

for i in $s1;
do echo "> T1/$i";
   ./$program < $t1/in/$i.in > $t1/res/$i.res
   
done;

rm -fv *

# T2
cp $program program;
cp $t2/*.bin $area;

for i in $s2;
do echo "> T2/$i";
   ./$program < $t2/in/$i.in > $t2/res/$i.res
done;

rm -fv *

# T3
cp $program program;
cp $t3/*.bin $area;

for i in $s3;
do echo "> T3/$i";
   ./$program < $t3/in/$i.in > $t3/res/$i.res
done;

rm -fv *

#Diff
for i in $s1;
do cmp -s $t1/out/$i.out $t1/res/$i.res || echo "failed on test T1/$i";
done;

for i in $s2;
do cmp -s $t2/out/$i.out $t2/res/$i.res || echo "failed on test T2/$i";
done;

for i in $s3;
do cmp -s $t3/out/$i.out $t3/res/$i.res || echo "failed on test T3/$i";
done;