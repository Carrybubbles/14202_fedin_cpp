#include "noisy.h"

int Noisy::noisy = 0;


std::ostream& operator<<(std::ostream& stream, const Noisy& noisy) {
    stream << noisy.value;
    return stream;
}
