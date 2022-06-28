#include "functions.h"

void save_directory(directory *data) {
    FILE *settings_file;
    settings_file = fopen("directory.txt", "w");
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        fprintf(settings_file, data->subscriber[i].first_name);
        fprintf(settings_file, data->subscriber[i].last_name);
        fprintf(settings_file, data->subscriber[i].phone_number);
        fprintf(settings_file, data->subscriber[i].second_name);
    }
    fclose(settings_file);
}

void read_directory(directory *data) {
    size_t len = 0, count = 0;
    FILE *settings_file;

    settings_file = fopen("directory.txt", "r");

    if (settings_file != NULL) {
        while (!feof(settings_file)) {
            fscanf(settings_file, "%*[^\n]%*c");
            count++;
        }
        fseek(settings_file, 0, SEEK_SET);
        data->count_subscriber = ((count - 1) / 4);
        for (unsigned int i = 0; i < data->count_subscriber; ++i) {
            getline(&data->subscriber[i].first_name, &len, settings_file);
            getline(&data->subscriber[i].last_name, &len, settings_file);
            getline(&data->subscriber[i].phone_number, &len, settings_file);
            getline(&data->subscriber[i].second_name, &len, settings_file);
        }
    }
    fclose(settings_file);
}

void memory_allocation_directory(directory *data) {
    data->subscriber = (subscriber *)calloc(255, sizeof(subscriber));
}

void remove_directory(directory *data) {
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        free(data->subscriber[i].second_name);
        free(data->subscriber[i].first_name);
        free(data->subscriber[i].last_name);
        free(data->subscriber[i].phone_number);
    }
    free(data->subscriber);
}

void print_subscriber(directory *data) {
    system("clear");
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        printf("subscriber[%u]\n", i);
        printf("Second name = %s", data->subscriber[i].second_name);
        printf("First name = %s", data->subscriber[i].first_name);
        printf("Last name = %s", data->subscriber[i].last_name);
        printf("phone number = %s", data->subscriber[i].phone_number);
        printf("------------------------------------------\n");
    }
}

void print_subscriber_i(directory *data, unsigned int *i) {
    system("clear");
    printf("subscriber[%u]\n", *i);
    printf("Second name = %s", data->subscriber[*i].second_name);
    printf("First name = %s", data->subscriber[*i].first_name);
    printf("Last name = %s", data->subscriber[*i].last_name);
    printf("phone number = %s", data->subscriber[*i].phone_number);
    printf("------------------------------------------\n");
}

void second_menu(directory *data) {
    system("clear");
    size_t len = 0;
    printf("Enter second name: ");
    getline(&data->subscriber[data->count_subscriber].second_name, &len, stdin);

    printf("Enter first name: ");
    getline(&data->subscriber[data->count_subscriber].first_name, &len, stdin);

    printf("Enter last name: ");
    getline(&data->subscriber[data->count_subscriber].last_name, &len, stdin);

    printf("Enter phone number: ");
    getline(&data->subscriber[data->count_subscriber].phone_number, &len, stdin);

    printf("------------------------------------------\n");
    printf("Subscriber added\n");
    ++data->count_subscriber;
}

void second_find(directory *data) {
    system("clear");
    int count_find = 0;
    printf("Enter second name : ");
    char *find_second_name = NULL;
    size_t len;
    getline(&find_second_name, &len, stdin);
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        if (strncmp(data->subscriber[i].second_name, find_second_name, len) == 0) {
            print_subscriber_i(data, &i);
            ++count_find;
        }
    }
    free(find_second_name);
    printf("Found [%d] subscribers\n", count_find);
}

void first_find(directory *data) {
    system("clear");
    int count_find = 0;
    printf("Enter first name : ");
    char *find_first_name = NULL;
    size_t len;
    getline(&find_first_name, &len, stdin);
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        if (strncmp(data->subscriber[i].first_name, find_first_name, len) == 0) {
            print_subscriber_i(data, &i);
            ++count_find;
        }
    }
    free(find_first_name);
    printf("Found [%d] subscribers\n", count_find);
}

void last_find(directory *data) {
    system("clear");
    int count_find = 0;
    printf("Enter last name : ");
    char *find_last_name = NULL;
    size_t len;
    getline(&find_last_name, &len, stdin);
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        if (strncmp(data->subscriber[i].last_name, find_last_name, len) == 0) {
            print_subscriber_i(data, &i);
            ++count_find;
        }
    }
    free(find_last_name);
    printf("Found [%d] subscribers\n", count_find);
}

void phone_find(directory *data) {
    system("clear");
    int count_find = 0;
    printf("Enter phone name : ");
    char *find_phone_number = NULL;
    size_t len;
    getline(&find_phone_number, &len, stdin);
    for (unsigned int i = 0; i < data->count_subscriber; ++i) {
        if (strncmp(data->subscriber[i].phone_number, find_phone_number, len) == 0) {
            print_subscriber_i(data, &i);
            ++count_find;
        }
    }
    free(find_phone_number);
    printf("Found [%d] subscribers\n", count_find);
}

void input_directory(directory *data) {
    data->count_subscriber = 0;
    memory_allocation_directory(data);
    int menu_key = -1;
    system("clear");
    read_directory(data);
    while (menu_key != 0) {
        printf("-----------------Directory----------------\n");
        printf("In the directory %u subscribers\n", data->count_subscriber);
        printf("Enter key 1 to display the directory\n");
        printf("Enter key 2 to add a subscriber\n");
        printf("Enter key 3 to search for a subscriber\n");
        printf("Enter 0 to exit the directory\n");
        scanf("%d", &menu_key);
        setbuf(stdin, NULL);
        if (menu_key == 1) {
            system("clear");
            if (data->count_subscriber == 0) {
                printf("Directory is empty!\n");
            } else {
                print_subscriber(data);
            }
        } else if (menu_key == 2) {
            second_menu(data);
        } else if (menu_key == 3) {
            int find_key = -1;
            printf("Enter search category: \n");
            printf("1 - second name, 2 - first name, 3 - last name, 4 - phone number\n");
            scanf("%d", &find_key);
            setbuf(stdin, NULL);
            if (find_key == 1) {
                second_find(data);
            } else if (find_key == 2) {
                first_find(data);
            } else if (find_key == 3) {
                last_find(data);
            } else if (find_key == 4) {
                phone_find(data);
            } else {
                system("clear");
                printf("syntax error!\n");
            }
        } else if (menu_key == 0) {
            system("clear");
        } else {
            system("clear");
            printf("syntax error!\n");
        }
    }
    save_directory(data);
    remove_directory(data);
}
