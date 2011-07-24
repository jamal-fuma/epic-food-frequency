search_path=${1:-./include}
ignore_path=${2:-'../include/boost*'}
output_path=${3:-./headers}

( \
find $search_path -wholename "$ignore_path" -prune -o  -type f \( -name '*.h' -o -name '*.hpp' \) -a -print | \
  while read name;
  do
     header_filename=${name/$search_path};
     source_filename=${name/$search_path/__DIR__};
     test_name=${source_filename/hpp/cpp};
     dir_name=$(dirname $source_filename);
     prefixed_name="test_${test_name/\//_}"
     prefixed_name="${prefixed_name/__DIR__}"

     include=$(echo '#include' "\"$header_filename\"";);
     main=$(printf '\nint\nmain(int argc,char **argv)\n{\n\treturn 0;\n}\n');

     test_case=$(printf "\n%s\n%s\n%s\n\n" "$include" "$include" "$main");
     cmd=$(printf "cat <<EOF > %s\n%s\nEOF\n" "$output_path/$prefixed_name" "$test_case")
     eval "$cmd"
  done
)

i=0;
file_list=$( find ${output_path} -name '*.cpp' | sort | sed -e 's/\..//; s/$//;')
for file in $file_list; do i=$(($i+1)); done;
max=$i; 

( \
  echo 'check_PROGRAMS = \'
i=0;
for file in $file_list; 
do 
  i=$(($i+1)); 
  name=$(basename $file);
  if [ $max -ne $i ] ; 
  then 
    printf '%40s \\\n' "$name";
  else
    printf '%40s\n' "$name";
  fi
done;
printf '\nTESTS=${check_PROGRAMS}\n' ;
) \
  | tee -a gen.sh >  ${output_path}/Makefile.am;
