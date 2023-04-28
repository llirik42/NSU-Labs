#pragma once

#include <map>
#include "utils.h"

using Row = Score;
using MatrixContent = std::map<Choices, Row>; // Rows of matrix

namespace AllChoices{
    inline const Step C = COOPERATION_STEP;
    inline const Step D = DEFECTION_STEP;

    inline const Choices CCC = {C, C, C};
    inline const Choices CCD = {C, C, D};
    inline const Choices CDC = {C, D, C};
    inline const Choices CDD = {C, D, D};
    inline const Choices DCC = {D, C, C};
    inline const Choices DCD = {D, C, D};
    inline const Choices DDC = {D, D, C};
    inline const Choices DDD = {D, D, D};
}

class Matrix{
public:
    Matrix(); // Creates default matrix

    explicit Matrix(const std::string& path);

    [[nodiscard]] bool has_error() const; // Have errors occurred during initialization

    [[nodiscard]] bool is_consistent() const; // Checks inequalities and symmetry

    [[nodiscard]] const Row& operator[] (const Choices& choices) const;

    [[nodiscard]] int get_element(const Choices& row_code, unsigned int index_in_row) const; // get element of row
private:
    bool _has_error;
    MatrixContent _content;

    Choices match_row(const std::string& line); // matches values of line and returns code of row

    bool input(std::ifstream& ifstream); // inputs matrix from stream

    [[nodiscard]] bool is_symmetric() const; // checks symmetry
};
