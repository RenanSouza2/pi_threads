while [ 1 ]
do
    du -sb numbers/ pieces/ | awk '{total += $1} END {print total}' | numfmt --to=iec
    sleep 60
done
