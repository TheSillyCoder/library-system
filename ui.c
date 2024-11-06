#include <string.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "ui.h"
#include "library.h"
#include "login.h"

struct state {
    int cx, cy, screenrows, screencols, numrows, rowoff;
    erow* row;
    struct termios orig_term;
    Book* books;
    int nbooks;
    char* username;
    int userPriv;
};
struct state E;

void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &E.orig_term) == -1) die("tcgetattr");
    atexit(disableRawMode);

    struct termios raw = E.orig_term;

    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL);
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

void die(char *s) {
    resetScreen();
    perror(s);
    exit(1);
}
void disableRawMode() {
    write(STDOUT_FILENO, "\x1b[?25h", 6); 
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_term) == -1) die("tcsetattr");
}
int readKeyPress() {
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1){
        if (nread == -1) die("read");
    }

    if (c == '\x1b') {
        char seq[3];

        if (read(STDIN_FILENO, &seq[0], 1) != 1) return '\x1b';
        if (read(STDIN_FILENO, &seq[1], 1) != 1) return '\x1b';

        if (seq[0] == '[') {
            if (seq[1] >= '0' && seq[1] <= '9') {
                if (read(STDIN_FILENO, &seq[2], 1) != 1) return '\x1b';
                if (seq[2] == '~') {
                    switch (seq[1]) {
                        case '1': return HOME_KEY;
                        case '3': return DEL_KEY;
                        case '4': return END_KEY;
                        case '5': return PAGE_UP;
                        case '6': return PAGE_DOWN;
                        case '7': return HOME_KEY;
                        case '8': return END_KEY;
                    }
                }
            } else {
                switch (seq[1]) {
                    case 'A': return ARROW_UP;
                    case 'B': return ARROW_DOWN;
                    case 'C': return ARROW_RIGHT;
                    case 'D': return ARROW_LEFT;
                    case 'H': return HOME_KEY;
                    case 'F': return END_KEY;
                }
            }
        } else if (seq[0] == 'O') {
            switch (seq[1]) {
                case 'H': return HOME_KEY;
                case 'F': return END_KEY;
            }
        }
    }
    return c;
}

void handleKeyPress() {
    int c = readKeyPress();
    switch (c) {
        case CTRL_Key('q'):
            resetScreen();
            disableRawMode();
            exit(0);
            break;
        case ARROW_UP:
        case ARROW_DOWN:
        case ARROW_LEFT:
        case ARROW_RIGHT:
            moveCursor(c);
            break;
        case '\r':
            moveCursor(ARROW_DOWN);
            break;
        default:
            break;
    }
}

void moveCursor(int c) {
    switch (c) {
        case ARROW_UP:
            if (E.cy > 0) E.cy--;
            break;
        case ARROW_DOWN:
            if (E.cy < E.numrows) E.cy++;
            break;
        case ARROW_LEFT:
            if (E.cx > 0) E.cx--;
            break;
        case ARROW_RIGHT:
            if (E.cx < E.screencols) E.cx++;
            break;
    }
    return;
}
void appendRow(char *s, size_t len) {
    E.row = realloc(E.row, sizeof(erow) * (E.numrows + 1));
    int at = E.numrows;
    E.row[at].size = len;
    E.row[at].chars = malloc(len + 1);
    memcpy(E.row[at].chars, s, len);
    E.row[at].chars[len] = '\0';
    E.numrows++;
}
void renderBooks() {
    E.books = fetchBooks("books-clean.csv", &E.nbooks);
    for (int i = 0; i < E.nbooks; ++i) {
        char rec[320];
        int len = snprintf(rec, sizeof(rec), "%-7d|%-55.55s|%-55.55s|%-55.55s|%d", i, E.books[i].title, E.books[i].authors, E.books[i].publisher, E.books[i].qty);
        appendRow(rec, len);
    }
}
void statusBar() {
    write(STDOUT_FILENO, "\x1b[7m", 4);
    char status[80];
    int len = snprintf(status, sizeof(status), "Library Management System");
    if (len > E.screencols) len = E.screencols;
    write(STDOUT_FILENO, status, len);
    char buf[255];
    int lenBuf = snprintf(buf, sizeof(buf), "Logged in as: '%s' [%d]", E.username, E.userPriv);
    for (int i = 0; i < E.screencols - len - lenBuf;++i) {
        write(STDOUT_FILENO, " ", 1);
    }
    write(STDOUT_FILENO, buf, lenBuf);
    write(STDOUT_FILENO, "\x1b[m", 3);
    write(STDOUT_FILENO, "\r\n", 2);
}

void topBar() {
    write(STDOUT_FILENO, "\x1b[7m", 4);
    char top[320];
    int len = snprintf(top, sizeof(top), "%-7s|%-55s|%-55s|%-55s|%s", "ID", "Title", "Authors", "Publishers", "Qty.");
    write(STDOUT_FILENO, top, len);
    for (int i = 0; i < E.screencols - len; ++i) write(STDOUT_FILENO, " ", 1);
    write(STDOUT_FILENO, "\x1b[m", 3);
    write(STDOUT_FILENO, "\r\n", 2);
}
void drawRows() {
    for (int y = 0 ; y < E.screenrows; ++y) {
        int filerow = y + E.rowoff;
        if (E.cy == filerow) write(STDOUT_FILENO, "\x1b[7m", 4);
        write(STDOUT_FILENO, E.row[filerow].chars, E.row[filerow].size);
        if (E.cy == filerow) write(STDOUT_FILENO, "\x1b[m", 3);
        write(STDOUT_FILENO, "\x1b[K", 3);
        write(STDOUT_FILENO, "\r\n", 2);
    }
}

void scroll() {
    if (E.cy < E.rowoff) {
        E.rowoff = E.cy;
        return;
    }
    if (E.cy >= E.rowoff + E.screenrows) {
        E.rowoff = E.cy - E.screenrows + 1;
    }
}
void goToxy(int x, int y) {
    char buf[32];
    snprintf(buf, sizeof(buf), "\x1b[%d;%dH", y, x);
    write(STDOUT_FILENO, buf, strlen(buf));
    return;
}

void resetScreen() {
    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

void refreshScreen() {
    scroll();
    write(STDOUT_FILENO, "\x1b[?25l", 6);
    resetScreen();
    topBar();
    drawRows();
    statusBar();
    goToxy(E.cx, E.cy - E.rowoff);
    /* write(STDOUT_FILENO, "\x1b[?25h", 6); */
    return;
}

void init() {
    E.rowoff = E.cx = E.cy = E.numrows = 0;
    if (login(&E.userPriv, &E.username) == LOGIN_FAILURE) {
        printf("Login Failed!!");
        exit(1);
        return;
    }
    renderBooks();
    enableRawMode();
    if (getWindowSize(&E.screenrows, &E.screencols) == -1) die("getWindowSize");
    E.screenrows -= 5;
    resetScreen();
}

int getWindowSize(int *rows, int *cols) {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) return -1;
        return getCursorPosition(rows, cols);
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
        return 0;
    }
}

int getCursorPosition(int *rows, int *cols) {
    char buf[32];
    unsigned int i = 0;
    if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) return -1;
    while (i < sizeof(buf) - 1) {
        if (read(STDIN_FILENO, &buf[i], 1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';
    if (buf[0] != '\x1b' || buf[1] != '[') return -1;
    if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) return -1;
    return 0;
}
