#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

declare -i passed_tests=0
declare -i failed_tests=0
declare -i test_num=0

echo "Failed :" > failed_tests.txt

# Fonction pour afficher le résultat du test en couleur
function print_result {
    if [[ "$1" == "pass" ]]; then
        echo -e "${GREEN}Pass${NC}"
    else
        echo -e "${RED}Fail${NC}"
    fi
}

# Fonction pour exécuter un test
function run_test {
    sleep 1
    input_cmd="$1"
    expected_output="$2"
    expected_status="$3"
    test_num="$4"

    # On exécute la commande sur bash
    bash_output=$(bash -c "$input_cmd" 2>&1)
    bash_status="$?"
    bash_output=$(echo "$bash_output" | sed 's/^bash: -c: at line 1 //')
    bash_output=$(echo "$bash_output" | sed 's/^bash: line [0-9]*: //')
    bash_output=$(echo "$bash_output" | sed 's/^bash: -c: line [0-9]*: //')
    expected_output=$bash_output


    # On exécute la commande sur minishell
    minishell_output=$(echo "$input_cmd" | ./minishell exit 2>&1)
    minishell_status="$?"
    minishell_output=$(echo "$minishell_output" | tail +2 | sed 's/^Minishell: //' | grep -av "heredoc >")

    # On compare les sorties et les status
    if [[ "$minishell_output" == "$expected_output" && "$minishell_status" -eq "$expected_status" ]]; then
        echo -e "\n\n${BLUE}Le test \"$input_cmd\" va être exécuté.${NC}"
        echo -e "Test $test_num: ${input_cmd} "
       # echo -e "  Bash output: ${GREEN}\n${bash_output}${NC}"
       # echo -e "  Bash status: ${GREEN}${bash_status}${NC}"
       # echo -e "  Minishell output: ${GREEN}\n${minishell_output}${NC}"
       # echo -e "  Minishell status: ${GREEN}${minishell_status}${NC}"
        print_result "pass"
        passed_tests+=1
    else
        echo -e "\n\n${BLUE}Le test \"$input_cmd\" va être exécuté.${NC}"
        echo -e "Test $test_num: ${input_cmd} "
       # echo "$expected_output && $expected_status"
        echo -e "  Bash output: ${RED}\n${bash_output}${NC}"
        echo -e "  Bash status: ${RED}${bash_status}${NC}"
        echo -e "  Minishell output: ${RED}\n${minishell_output}${NC}"
        echo -e "  Minishell status: ${RED}${minishell_status}${NC}"
        print_result "fail"
        failed_tests+=1
        echo -e "\n\n${BLUE}Le test \"$input_cmd\" va être exécuté.${NC}" >> failed_tests.txt
        echo -e "Test $test_num: ${input_cmd} " >> failed_tests.txt
        echo -e "  Bash output: ${RED}\n${bash_output}${NC}" >> failed_tests.txt
        echo -e "  Bash status: ${RED}${bash_status}${NC}" >> failed_tests.txt
        echo -e "  Minishell output: ${RED}\n${minishell_output}${NC}" >> failed_tests.txt
        echo -e "  Minishell status: ${RED}${minishell_status}${NC}" >> failed_tests.txt
    fi
}

# On parcourt chaque ligne du fichier tests.txt
test_num=0
while read -r line || [[ -n "$line" ]]; do
    test_num+=1
    input_cmd="$line"
    expected_output=$(bash -c "$input_cmd" 2>&1)
    expected_status="$?"

    # On exécute le test
    run_test "$input_cmd" "$expected_output" "$expected_status" "$test_num"
done < tests.txt
# On affiche le nombre de tests réussis et échoués
echo -e "\nSummary:"
echo -e "Passed tests: ${GREEN}${passed_tests}${NC}"
echo -e "Failed tests: ${RED}${failed_tests}${NC}"
echo -e "\n 'cat failed_tests.txt' to see the failed tests."

# On quitte le programme avec le code de sortie approprié
if [[ "$failed_tests" -gt 0 ]]; then
    exit 1
else
    exit 0
fi

# On affiche le nombre de tests réussis et échoués
echo -e "\nSummary:"
echo -e "Passed

