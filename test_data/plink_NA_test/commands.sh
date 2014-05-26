bgiplink --file test1 --make-bed --out test1
bgiplink --allow-no-sex --bfile test1  --linear hide-covar --out test1 --pheno test1.phe --pheno-name height
cat test1.assoc.linear
