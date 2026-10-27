// src/tools/calculator_eval.cpp — YOU IMPLEMENT (spec §4.6).
//
// calc_evaluate: evaluate a well-formed token list with two stacks
// (one for numbers, one for operators and '('). See spec §4.6 and the
// worked trace in Appendix C. std::stack or std::vector are both fine.

#include "tools/calculator.h"

bool calc_evaluate(const std::vector<CalcToken>& tokens, double* out,
                   std::string* error) {
    using Kind = CalcToken::Kind;
    std::vector<double> values;
    std::vector<Kind> operators;

    const auto fail = [error](const char* message) {
        *error = message;
        return false;
    };
    const auto precedence = [](Kind op) {
        if (op == Kind::Times || op == Kind::Divide) return 2;
        if (op == Kind::Plus || op == Kind::Minus) return 1;
        return 0;
    };
    const auto apply_top = [&]() {
        if (operators.empty() || operators.back() == Kind::LParen) {
            return fail("missing operator");
        }
        if (values.size() < 2) return fail("not enough operands");

        const Kind op = operators.back();
        operators.pop_back();
        const double right = values.back();
        values.pop_back();
        const double left = values.back();
        values.pop_back();

        double result = 0.0;
        switch (op) {
        case Kind::Plus: result = left + right; break;
        case Kind::Minus: result = left - right; break;
        case Kind::Times: result = left * right; break;
        case Kind::Divide:
            if (right == 0.0) return fail("division by zero");
            result = left / right;
            break;
        default: return fail("invalid operator");
        }
        values.push_back(result);
        return true;
    };

    for (const CalcToken& token : tokens) {
        switch (token.kind) {
        case Kind::Number:
            values.push_back(token.value);
            break;
        case Kind::LParen:
            operators.push_back(token.kind);
            break;
        case Kind::RParen:
            while (!operators.empty() && operators.back() != Kind::LParen) {
                if (!apply_top()) return false;
            }
            if (operators.empty()) return fail("unmatched closing parenthesis");
            operators.pop_back();
            break;
        case Kind::Plus:
        case Kind::Minus:
        case Kind::Times:
        case Kind::Divide:
            while (!operators.empty() && operators.back() != Kind::LParen &&
                   precedence(operators.back()) >= precedence(token.kind)) {
                if (!apply_top()) return false;
            }
            operators.push_back(token.kind);
            break;
        default:
            return fail("invalid token");
        }
    }

    while (!operators.empty()) {
        if (operators.back() == Kind::LParen) return fail("unmatched opening parenthesis");
        if (!apply_top()) return false;
    }
    if (values.size() != 1) return fail("invalid expression");

    *out = values.back();
    return true;
}
