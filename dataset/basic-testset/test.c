int arg();
int main() {
    if (arg() >= 0)
        return 0;
    else
        return 1;
}

int arg() {
    int j = 0;
    if (j != 5) {
        if (j < 3) {
            if (j > -1) {
                if (j == 0) {
                    return 0;
                } else {
                    j++;
                }
            } else {
                j *= 4;
            }
        } else {
            j -= 5;
        }
    } else {
        j += 3;
    }
    return -1;
}