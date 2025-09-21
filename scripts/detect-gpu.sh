#! /bin/bash
echo $(lspci -mm | awk -F '\"|\" \"|\\(' \
'/"Display|"3D|"VGA/ {
a[$0] = $1 " " $3 " " ($(NF-1) ~ /^$|^Device [[:xdigit:]]+$/ ? $4 : $(NF-1))
}
END { for (i in a) {
if (!seen[a[i]]++) {
sub("^[^ ]+ ", "", a[i]);
print a[i]
}
}}')
