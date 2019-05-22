export FIGDIR=figures

#for f in scripts/fig*.sh; do
#    $f
#done

parallel -- scripts/fig*.sh
