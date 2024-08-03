#pragma once
#include <string>
#include <string_view>
#include <algorithm>

#include "token.hpp"

namespace sk {
	class tokenizer {
	public:
		tokenizer(std::string source);
		token next_token();

	private:
		static bool is_digit(char c);
		static bool is_alpha(char c);
		static bool is_space(char c);

		bool is_at_end() const;
		char peek() const;
		char peek_next() const;
		char advance();
		bool match(char expected);
		void skip_whitespace();

		std::string_view get_view() const;

		template <typename Predicate>
		void advance_while(Predicate predicate) {
			while (!is_at_end() && predicate(peek())) {
				advance();
			}
		}

		token consume_number();
		token consume_string();
		token consume_identifier();

		token create_token(token_type type);

	private:
		std::string m_source;
		size_t m_start;
		size_t m_current;
	};
}