#!/usr/bin/env python3

import os

homedir = os.environ["HOME"]
rcfile = os.path.join(homedir, ".plinkcoll.rc")
if os.path.exists(rcfile):
    with open(rcfile) as rcfh:
        plinkpath = rcfh.readline().strip()
else:
    raise Exception("You should give path to plink binary in " + rcfile + " !")

if not os.path.exists(plinkpath) or not os.access(plinkpath, os.X_OK):
    raise Exception("Sorry, the plink path either does not exist or is not executable!")


plinkname = os.path.basename(plinkpath)
collname = "plinkcoll.py"
completion_string_plink = """
_plink()
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--1 --23file --a1-allele --a2-allele --adjust --aec --allele1234 --alleleacgt --allele-count --allow-extra-chr --allow-no-sex --all-pheno --aperm --assoc --autosome --autosome-num --autosome-xy --bcf --bed --bfile --bgen --biallelic-only --bim --blocks --blocks-inform-frac --blocks-max-kb --blocks-min-maf --blocks-recomb-highci --blocks-strong-highci --blocks-strong-lowci --bmerge --bp-space --cell --check-sex --chr --chr-set --ci --clump --clump-allow-overlap --clump-annotate --clump-best --clump-field --clump-index-first --clump-kb --clump-p1 --clump-p2 --clump-r2 --clump-range --clump-range-border --clump-replicate --clump-snp-field --clump-verbose --cluster --cm-map --complement-sets --condition --condition-list --const-fid --covar --covar-name --covar-number --cow --d --data --debug --distance --distance-exp --distance-matrix --double-id --dummy --dummy-coding --epi1 --epi2 --epistasis --epistasis-summary-merge --exclude --exclude-snp --exclude-snps --extract --fam --family --fast-epistasis --file --filter --filter-attrib --filter-attrib-indiv --filter-cases --filter-controls --filter-females --filter-founders --filter-males --filter-nonfounders --flip --flipscan --flip-scan --flip-scan-threshold --flip-scan-window --flip-scan-window-kb --flip-subset --freq --freqx --from --from-bp --gap --gen --gene --gene-all --geno --genome --gplink --grm-bin --grm-cutoff --grm-gz --groupdist --gxe --hap --hard-call-threshold --hardy --het --homozyg --homozyg-density --homozyg-gap --homozyg-het --homozyg-kb --homozyg-match --homozyg-snp --homozyg-window-het --homozyg-window-missing --homozyg-window-snp --homozyg-window-threshold --hwe --ibc --ibm --ibs-matrix --ibs-test --id-delim --impute-sex --indep --indep-pairwise --indiv-sort --je-cellmin --k --keep --keep-allele-order --keep-autoconv --keep-cluster-names --keep-clusters --keep-fam --lambda --lasso --lasso-select-covars --ld --ld-snp --ld-snp-list --ld-snps --ld-window --ld-window-kb --ld-window-r2 --ld-xchr --lfile --linear --list-23-indels --logistic --loop-assoc --maf --maf-succ --make-bed --make-founders --make-grm-bin --make-grm-gz --make-perm-pheno --make-pheno --make-rel --make-set --make-set-border --make-set-collapse-group --make-set-complement-all --map --match --match-type --max --max-maf --mc --mcc --mds-plot --me --me-exclude-one --memory --mendel --mendel-duos --mendel-multigen --merge --merge-equal-pos --merge-list --merge-mode --merge-x --mfilter --min --mind --missing --missing_code --missing-code --missing-genotype --missing-phenotype --missing-var-code --model --mperm-save --mperm-save-all --mpheno --must-have-sex --mwithin --neighbor --neighbour --no-fid --nonfounders --no-parents --no-pheno --no-sex --not-chr --oblig-missing --out --output-chr --output-missing-genotype --output-missing-phenotype --parallel --parameters --pca --pca-cluster-names --pca-clusters --ped --perm-batch-size --pfilter --pheno --pheno-merge --pheno-name --pool-size --ppc --ppc-gap --prune --qmatch --q-score-range --qt --r --r2 --read-dists --read-freq --read-genome --recode --recode-allele --reference --regress-distance --regress-rel --rel-cutoff --remove --remove-cluster-names --remove-clusters --remove-fam --rerun --sample --score --script --seed --set --set-collapse-all --set-hh-missing --set-max --set-me-missing --set-missing-nonsnp-ids --set-missing-snp-ids --set-missing-var-ids --set-names --set-p --set-r2 --set-table --silent --simulate --simulate-label --simulate-missing --simulate-n --simulate-ncases --simulate-ncontrols --simulate-prevalence --simulate-qt --snp --snps --snps-only --split-x --subset --tail-pheno --tdt --test-mishap --test-missing --tests --tfam --tfile --thin --thin-count --threads --to --to-bp --tped --twolocus --update-alleles --update-chr --update-cm --update-ids --update-map --update-name --update-parents --update-sex --vcf --vcf-filter --vcf-idspace-to --vcf-min-qual --version --vif --window --within --with-phenotype --write-cluster --write-covar --write-set --write-snplist --xchr-model --zero-cluster --zero-cms --hide-covar --help"

    if [[ ${prev} == --*file ]] || [[ ${prev} == --out ]] || [[ $prev == --covar ]] || [[ $prev == --pheno ]]; then
        COMPREPLY=( $(compgen -o filenames -G "$cur*") )
    elif [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _plink """ + plinkname + "\n\n\n"

completion_string_coll = """
_plinkcoll.py()
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"
    opts="--1 --23file --a1-allele --a2-allele --adjust --aec --allele1234 --alleleacgt --allele-count --allow-extra-chr --allow-no-sex --all-pheno --aperm --assoc --autosome --autosome-num --autosome-xy --bcf --bed --bfile --bgen --biallelic-only --bim --blocks --blocks-inform-frac --blocks-max-kb --blocks-min-maf --blocks-recomb-highci --blocks-strong-highci --blocks-strong-lowci --bmerge --bp-space --cell --check-sex --chr --chr-set --ci --clump --clump-allow-overlap --clump-annotate --clump-best --clump-field --clump-index-first --clump-kb --clump-p1 --clump-p2 --clump-r2 --clump-range --clump-range-border --clump-replicate --clump-snp-field --clump-verbose --cluster --cm-map --complement-sets --condition --condition-list --const-fid --covar --covar-name --covar-number --cow --d --data --debug --distance --distance-exp --distance-matrix --double-id --dummy --dummy-coding --epi1 --epi2 --epistasis --epistasis-summary-merge --exclude --exclude-snp --exclude-snps --extract --fam --family --fast-epistasis --file --filter --filter-attrib --filter-attrib-indiv --filter-cases --filter-controls --filter-females --filter-founders --filter-males --filter-nonfounders --flip --flipscan --flip-scan --flip-scan-threshold --flip-scan-window --flip-scan-window-kb --flip-subset --freq --freqx --from --from-bp --gap --gen --gene --gene-all --geno --genome --gplink --grm-bin --grm-cutoff --grm-gz --groupdist --gxe --hap --hard-call-threshold --hardy --het --homozyg --homozyg-density --homozyg-gap --homozyg-het --homozyg-kb --homozyg-match --homozyg-snp --homozyg-window-het --homozyg-window-missing --homozyg-window-snp --homozyg-window-threshold --hwe --ibc --ibm --ibs-matrix --ibs-test --id-delim --impute-sex --indep --indep-pairwise --indiv-sort --je-cellmin --k --keep --keep-allele-order --keep-autoconv --keep-cluster-names --keep-clusters --keep-fam --lambda --lasso --lasso-select-covars --ld --ld-snp --ld-snp-list --ld-snps --ld-window --ld-window-kb --ld-window-r2 --ld-xchr --lfile --linear --list-23-indels --logistic --loop-assoc --maf --maf-succ --make-bed --make-founders --make-grm-bin --make-grm-gz --make-perm-pheno --make-pheno --make-rel --make-set --make-set-border --make-set-collapse-group --make-set-complement-all --map --match --match-type --max --max-maf --mc --mcc --mds-plot --me --me-exclude-one --memory --mendel --mendel-duos --mendel-multigen --merge --merge-equal-pos --merge-list --merge-mode --merge-x --mfilter --min --mind --missing --missing_code --missing-code --missing-genotype --missing-phenotype --missing-var-code --model --mperm-save --mperm-save-all --mpheno --must-have-sex --mwithin --neighbor --neighbour --no-fid --nonfounders --no-parents --no-pheno --no-sex --not-chr --oblig-missing --out --output-chr --output-missing-genotype --output-missing-phenotype --parallel --parameters --pca --pca-cluster-names --pca-clusters --ped --perm-batch-size --pfilter --pheno --pheno-merge --pheno-name --pool-size --ppc --ppc-gap --prune --qmatch --q-score-range --qt --r --r2 --read-dists --read-freq --read-genome --recode --recode-allele --reference --regress-distance --regress-rel --rel-cutoff --remove --remove-cluster-names --remove-clusters --remove-fam --rerun --sample --score --script --seed --set --set-collapse-all --set-hh-missing --set-max --set-me-missing --set-missing-nonsnp-ids --set-missing-snp-ids --set-missing-var-ids --set-names --set-p --set-r2 --set-table --silent --simulate --simulate-label --simulate-missing --simulate-n --simulate-ncases --simulate-ncontrols --simulate-prevalence --simulate-qt --snp --snps --snps-only --split-x --subset --tail-pheno --tdt --test-mishap --test-missing --tests --tfam --tfile --thin --thin-count --threads --to --to-bp --tped --twolocus --update-alleles --update-chr --update-cm --update-ids --update-map --update-name --update-parents --update-sex --vcf --vcf-filter --vcf-idspace-to --vcf-min-qual --version --vif --window --within --with-phenotype --write-cluster --write-covar --write-set --write-snplist --xchr-model --zero-cluster --zero-cms --hide-covar --help --plinkcoll-range"

    if [[ ${prev} == --*file ]] || [[ ${prev} == --out ]] || [[ $prev == --covar ]] || [[ $prev == --pheno ]]; then
        COMPREPLY=( $(compgen -o filenames -G "$cur*") )
    elif [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi
}
complete -F _plinkcoll.py """ + collname + "\n\n\n"



if not os.path.exists("/etc/bash_completion.d"):
    os.mkdir("/etc/bash_completion.d")

completion_path_plink = "/etc/bash_completion.d/%s" % plinkname
if os.path.isfile(completion_path_plink):
    print("Completion script %s already exists." % completion_path_plink)
    comp_reply = input("Type yes if you want to overwrite: ")
    if comp_reply.strip() != "yes":
        print("I leave the original file alone, as you have requested. ")
        pass

completion_path_coll = "/etc/bash_completion.d/plinkcoll.py"
if os.path.exists(completion_path_coll):
    print("Completion script %s already exists." % completion_path_coll)
    comp_reply = input("Type yes if you want to overwrite: ")
    if comp_reply.strip() != "yes":
        print("I leave the original file alone, as you have requested.")
        exit(0)

try:
    with open("", "a") as fh:
        fh.write(completion_string_plink)
except PermissionError:
    print("Permission denied while trying to write to file:")
    print(completion_path_plink)
    print("Note that this command requires root access (administrator).")
    print("I suggest you put 'sudo' at the beginning.")

try:
    with open(completion_path_coll, "w") as fh_coll:
        fh_coll.write(completion_string_coll)
except PermissionError:
    print("Permission denied while trying to write to file:")
    print(completion_path_coll)
    print("Note that this command requires root access (administrator).")
    print("I suggest you put 'sudo' at the beginning.")
