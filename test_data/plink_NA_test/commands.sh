plink --file test1 --make-bed --out test1
plink --allow-no-sex --bfile test1  --linear --out test1 --pheno test1.phe --pheno-name height
cat test1.assoc.linear


plink --file test2 --make-bed --out test2
plink --allow-no-sex --bfile test2  --linear --out test2 --pheno test2.phe --pheno-name height
cat test2.assoc.linear
