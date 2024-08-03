#include "tokenizer.hpp"
#include <unordered_map>

namespace sk {
	tokenizer::tokenizer(std::string source)
		: m_source(source), m_start(0), m_current(0) {}

	token tokenizer::next_token() {
		skip_whitespace();

		m_start = m_current;

		if (is_at_end()) {
			return create_token(token_type::END_OF_FILE);
		}

		char c = advance();

		if (is_digit(c)) {
			return consume_number();
		}

		if (is_alpha(c)) {
			return consume_identifier();
		}

		switch (c) {
			case '{':  return create_token(token_type::LBRACE);
			case '}':  return create_token(token_type::RBRACE);
			case '[':  return create_token(token_type::LBRACKET);
			case ']':  return create_token(token_type::RBRACKET);
			case '(':  return create_token(token_type::LPAREN);
			case ')':  return create_token(token_type::RPAREN);
			case ':':  return create_token(token_type::COLON);
			case ';':  return create_token(token_type::SEMICOLON);
			case ',':  return create_token(token_type::COMMA);
			case '+':  return create_token(token_type::PLUS);
			case '-':  return create_token(token_type::MINUS);
			case '*':  return create_token(token_type::STAR);
			case '/':  return create_token(token_type::SLASH);

			case '.':
				if (match('.')) {
					if (match('=')) {
						return create_token(token_type::INCLUSIVE_RANGE);
					}
					else {
						return create_token(token_type::EXCLUSIVE_RANGE);
					}
				}
				else {
					return create_token(token_type::DOT);
				}

			case '!':
				if (match('=')) {
					return create_token(token_type::BANG_EQUAL);
				}
				else {
					return create_token(token_type::BANG);
				}

			case '=':
				if (match('=')) {
					return create_token(token_type::EQUAL_EQUAL);
				}
				else {
					return create_token(token_type::EQUAL);
				}

			case '<':
				if (match('=')) {
					return create_token(token_type::LESS_EQUAL);
				}
				else {
					return create_token(token_type::LESS);
				}

			case '>':
				if (match('=')) {
					return create_token(token_type::GREATER_EQUAL);
				}
				else {
					return create_token(token_type::GREATER);
				}

			case '"': return consume_string();

			default:
				break;
		}

		return create_token(token_type::ERROR_UNEXPECTED_CHARACTER);
	}

	bool tokenizer::is_digit(char c) {
		return c >= '0' && c <= '9';
	}

	bool tokenizer::is_alpha(char c) {
		return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
	}

	bool tokenizer::is_space(char c) {
		return c == ' ' || c == '\t' || c == '\r' || c == '\n';
	}

	bool tokenizer::is_at_end() const {
		return m_current >= m_source.length();
	}

	char tokenizer::peek() const {
		return m_source[m_current];
	}

	char tokenizer::peek_next() const {
		if (is_at_end()) {
			return '\0';
		}

		return m_source[m_current + 1];
	}

	char tokenizer::advance() {
		return m_source[m_current++];
	}

	bool tokenizer::match(char expected) {
		if (peek() == expected) {
			advance();
			return true;
		}
		else {
			return false;
		}
	}

	void tokenizer::skip_whitespace() {
		advance_while(is_space);
	}

	std::string_view tokenizer::get_view() const {
		return std::string_view(m_source.data() + m_start, m_current - m_start);
	}

	token tokenizer::consume_number() {
		advance_while(is_digit);

		if (peek() == '.' && is_digit(peek_next())) {
			// Consume the period.
			advance();

			// Consume the rest of the number.
			advance_while(is_digit);
		}

		return create_token(token_type::NUMBER);
	}

	token tokenizer::consume_string() {
		advance_while([](char c) {
			return c != '"';
		});

		if (is_at_end()) {
			return create_token(token_type::ERROR_UNTERMINATED_STRING);
		}

		// Consume the closing quote.
		advance();

		return create_token(token_type::STRING);
	}

	token tokenizer::consume_identifier() {
		advance_while([](char c) {
			return is_digit(c) || is_alpha(c);
		});

		static const std::unordered_map<std::string_view, token_type> keyword_map = {
			{"and",    token_type::AND},
			{"or",     token_type::OR},
			{"fn",     token_type::FN},
			{"if",     token_type::IF},
			{"else",   token_type::ELSE},
			{"elif",   token_type::ELIF},
			{"return", token_type::RETURN},
			{"let",    token_type::LET},
			{"while",  token_type::WHILE},
			{"for",    token_type::FOR},
			{"in",     token_type::IN},
			{"true",   token_type::TRUE},
			{"false",  token_type::FALSE},
			{"none",   token_type::NONE},
		};

		std::string_view view = get_view();

		if (keyword_map.contains(view)) {
			return create_token(keyword_map.at(view));
		}
		else {
			return create_token(token_type::IDENTIFIER);
		}
	}

	token tokenizer::create_token(token_type type) {
		return token(type, get_view());
	}
}