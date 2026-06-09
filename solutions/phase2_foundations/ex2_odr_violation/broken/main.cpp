// Phase 2, Exercise 2 — ODR Violation (BROKEN version)

void do_a_thing();
void do_b_thing();

int main() {
    do_a_thing();
    do_b_thing();
    return 0;
}
