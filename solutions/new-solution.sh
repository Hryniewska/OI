#!/bin/bash

SOLUTION=$1

mkdir -p ${SOLUTION}/{src,bin,deploy,data,run}

for script in ${SOLUTION}/{build,deploy}.sh; do
    cat > $script <<SCRIPT
#!/bin/bash
echo "pusty skrypt!"
exit 1
SCRIPT

    chmod +x $script

done
