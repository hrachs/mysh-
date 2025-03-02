#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

char* foo(char* buff) {
    // Ստեղծում ենք տողը բաժանող փոփոխություն
    char* token = strtok(buff, " ");
    return token;  // Վերադարձնում ենք առաջին token-ը
}

int main() {
    char exit_word[] = "exit";  // exit_word կոմանդը
    char arg[50] = {};  // Պարամետրերի զանգվածը
    char* buff = (char *)malloc(100 * sizeof(char));  // Մուտքագրված տողը պահելու համար
    do {
        printf("mysh>");
        fgets(buff, 100, stdin);  // Մենք օգտագործում ենք fgets(), որպեսզի ամբողջ տողը վերցնենք (ներառյալ տարածքները)

        // Դուրս ենք բերում նոր տողի վերջը
        buff[strcspn(buff, "\n")] = 0;

        char* command = foo(buff);  // Առաջին token-ը՝ հիմնական կոմանդը

        if (strcmp(exit_word, command) == 0) {  // Եթե կոմանդը "exit_word" է, դուրս ենք գալիս
            free(buff);  // Ազատում ենք հիշողությունը
            return 0;
        } else {
            pid_t pid = fork();  // Նոր պրոցեսի ստեղծում

            if (pid > 0) {
                // Հոր պրոցեսը սպասում է երեխային
                wait(NULL);
            } else if (pid == 0) {
                // Երեխա պրոցեսը
                int j = 0;
                // Կրկնվող ստուգում՝ `command` պարամետրերի համար
                for (int i = 0; command[i] != '\0'; i++) {
                    if (command[i] == '-') {  // Ստուգում ենք, եթե կա `-`
                        i++;  // Ապա նորմալ պարամետրը կցվում է

                        // Ակնկալվում է պարամետրերի հավաքագրում
                        while (command[i] != '\0' && command[i] != ' ') {
                            arg[j++] = command[i++];  // Պարամետրը համակցվում է
                        }
                    }
                }

                // execlp() օգտագործելով հրամանը և պարամետրերը
                execlp(command, command, arg, NULL);

                // execlp() երբեք չի պետք է հասնի այս կետի, քանի որ այն փոխում է պրոցեսը:
                perror("execlp failed");
                exit(1);  // Եթե execlp-ը ձախողվի
            }
        }

    } while (1);

    free(buff);  // Ազատում ենք մուտքագրված տողի հիշողությունը
    return 0;
}
