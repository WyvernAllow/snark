#pragma once
#include <cstddef>
#include <string>

namespace sk {
	enum token_type {
		LBRACE,
		RBRACE,
		RBRACKET,
		LBRACKET,
		LPAREN,
		RPAREN,
		COLON,
		SEMICOLON,
		COMMA,
		PLUS,
		MINUS,
		STAR,
		SLASH,
		BANG,
		BANG_EQUAL,
		EQUAL,
		EQUAL_EQUAL,
		LESS,
		LESS_EQUAL,
		GREATER,
		GREATER_EQUAL,
		DOT,
		INCLUSIVE_RANGE,
		EXCLUSIVE_RANGE,
		IDENTIFIER,
		STRING,
		NUMBER,
		AND,
		OR,
		FN,
		IF,
		ELSE,
		ELIF,
		RETURN,
		LET,
		WHILE,
		FOR,
		IN,
		TRUE,
		FALSE,
		NONE,
		ERROR_UNEXPECTED_CHARACTER,
		ERROR_UNTERMINATED_STRING,
		END_OF_FILE,
	};

	std::string token_type_to_string(token_type type);

	struct token {
		token_type type;
		std::string_view lexeme;

		token(token_type type, std::string_view lexeme)
			: type(type), lexeme(lexeme) {}
	};
}