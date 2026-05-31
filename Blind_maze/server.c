#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 2048

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char display_buffer[BUFFER_SIZE] = {0};

    // 1. Creare socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 2. Bind & Listen
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    }

    printf("Serverul Blind Maze a pornit pe portul %d. Asteptare client...\n", PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept");
        exit(EXIT_FAILURE);
    }

    printf("Client conectat!\n");

    GameState game;
    init_game(&game);

    // Bucla de joc a serverului
    while (1) {
        // Generam starea vizuala si o trimitem clientului
        render_game_to_buffer(&game, display_buffer);
        send(new_socket, display_buffer, strlen(display_buffer), 0);

        // Verificam daca s-a terminat
        if (game.player.has_won || !game.player.is_alive) {
            break;
        }

        // Asteptam actiunea de la client
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(new_socket, buffer, BUFFER_SIZE);
        
        if (valread <= 0) {
            printf("Clientul s-a deconectat.\n");
            break;
        }

        char input = buffer[0];
        if (input == 'q') {
            printf("Jucatorul a parasit jocul.\n");
            break;
        }

        // Procesam "tick-ul" de actiune
        process_tick(&game, input);
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
