#ifndef CONFIG_LINE_HPP
#define CONFIG_LINE_HPP

#include "utils/ft_string.hpp"

#include <string>
#include <vector>

namespace config {

#define LINE_COMMENT "#"

#define BLOCK_SERVER   "server"
#define BLOCK_LOCATION "location"
#define BLOCK_END      "}"

#define DIRECTIVE_LISTEN        "listen"
#define DIRECTIVE_SERVER_NAME   "server_name"
#define DIRECTIVE_ROOT          "root"
#define DIRECTIVE_ERROR_PAGE    "error_page"
#define DIRECTIVE_MAX_BODY_SIZE "client_max_body_size"
#define DIRECTIVE_ALLOW_METHODS "allow_methods"
#define DIRECTIVE_INDEX         "index"
#define DIRECTIVE_AUTOINDEX     "autoindex"
#define DIRECTIVE_FASTCGI       "fastcgi_pass"

typedef enum {
    Empty,
    Block,
    Route,
    End,
    Listen,
    ServerName,
    Root,
    ErrorPage,
    MaxBodySize,
    AllowMethods,
    Index,
    AutoIndex,
    CgiExtension,
    Unknown
} LineType;

class Line {
    public:

        Line(void);
        Line(Line const& src);
        Line(std::string const& line);

        ~Line(void);

        Line& operator=(Line const& rhs);

        std::string getLine(void) const;
        LineType    getType(void) const;

        std::vector<std::string> format(void) const;

    private:

        std::string m_line;
        LineType    m_type;
};

LineType convertLineType(std::string const& line);

} // namespace config

#endif /* CONFIG_LINE_HPP */