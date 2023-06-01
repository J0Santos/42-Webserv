#include "config/Options.hpp"
#include "config/Parser.hpp"

#include <gtest/gtest.h>

class testOptionsExtreme : public ::testing::Test {
    protected:

        testOptionsExtreme(void) {
            system("touch /tmp/testsOptions.tmp");
            system("echo \"server {\n"
                   "  listen 8080:localhost;\n"
                   "  root          ./websites/;\n"
                   "  allow_methods GET POST;\n"
                   "\n"
                   "  location / {\n"
                   "    root     ./websites/cgi/;\n"
                   "    autoindex on;\n"
                   "  }\n"
                   "\n"
                   "  location /python/ {\n"
                   "    root          ./websites/cgi/python;\n"
                   "    index         ./websites/index.html;\n"
                   "    fastcgi_pass  .py;\n"
                   "    allow_methods GET;\n"
                   "  }\n"
                   "\n"
                   "  location php/ {\n"
                   "    fastcgi_pass .php;\n"
                   "    root         ./websites/cgi/php;\n"
                   "  }\n"
                   "}\n"
                   "server {\n"
                   "  listen               8081;\n"
                   "  server_name          domain.com;\n"
                   "  root                 ./websites/;\n"
                   "  error_page 404       ./websites/errors/404.html;\n"
                   "  allow_methods        GET POST;\n"
                   "  client_max_body_size 80;\n"
                   "}\n"
                   "\n"
                   "server {\n"
                   "  listen         8081:localhost;\n"
                   "  root           ./websites/;\n"
                   "  server_name    domain.net;\n"
                   "  error_page 500 ./websites/errors/500.html;\n"
                   "}\" >> /tmp/testsOptions.tmp");
            config::parse("/tmp/testsOptions.tmp");
        }

        ~testOptionsExtreme(void) { system("rm /tmp/testsOptions.tmp"); }
};

TEST_F(testOptionsExtreme, testServerOptsExtractionFromOptions) {
    // tommorrow
}

TEST_F(testOptionsExtreme, testGettingFirstBlockWithDefaultRoute) {

    smt::shared_ptr<config::Opts> opt(new config::Opts);
    opt->m_target = "/";
    opt->m_host = "127.0.0.1";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = true;
    opt->m_cgi_extension = "";

    ASSERT_EQ(*config::Options::getOptions("8080", "127.0.0.1", "/"), *opt);
}

TEST_F(testOptionsExtreme, testGettingFirstBlockPythonRouted) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/python/";
    opt->m_host = "127.0.0.1";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/python/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET"});
    opt->m_index = "./websites/index.html";
    opt->m_autoindex = false;
    opt->m_cgi_extension = ".py";
    ASSERT_EQ(
        *config::Options::getOptions("8080", "127.0.0.1", "/python/me.py"),
        *opt);
}

TEST_F(testOptionsExtreme, testGettingFirstBlockPhRoutped) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/php/";
    opt->m_host = "127.0.0.1";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/php/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = false;
    opt->m_cgi_extension = ".php";
    ASSERT_EQ(*config::Options::getOptions("8080", "127.0.0.1", "/php/me.php"),
              *opt);
}

TEST_F(testOptionsExtreme, testGettingSecondBlockWithDefaultRoute) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/";
    opt->m_host = "127.0.0.1";
    opt->m_port = "8081";
    opt->m_root = "./websites/";
    opt->m_server_name = "domain.com";
    opt->m_error_pages[404] = "./websites/errors/404.html";
    opt->m_max_body_size = 80;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = false;
    opt->m_cgi_extension = "";
    ASSERT_EQ(*config::Options::getOptions("8081", "127.0.0.1", "/"), *opt);
    ASSERT_EQ(
        *config::Options::getOptions("8081", "127.0.0.1", "/", "domain.com"),
        *opt);
    ASSERT_EQ(*config::Options::getOptions("8081", "127.0.0.1", "/python/",
                                           "domain.pt"),
              *opt);
}

TEST_F(testOptionsExtreme, testThirdBlock) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/";
    opt->m_host = "127.0.0.1";
    opt->m_port = "8081";
    opt->m_root = "./websites/";
    opt->m_server_name = "domain.net";
    opt->m_error_pages[500] = "./websites/errors/500.html";
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods =
        std::vector<std::string>({"GET", "POST", "DELETE"});
    opt->m_index = "";
    opt->m_autoindex = false;
    opt->m_cgi_extension = "";
    ASSERT_EQ(
        *config::Options::getOptions("8081", "127.0.0.1", "/", "domain.net"),
        *opt);
}

TEST_F(testOptionsExtreme, testGetSocketOptions) {
    std::set< std::pair<int, std::string> > socketOpts;
    socketOpts.insert(std::make_pair(8080, "127.0.0.1"));
    socketOpts.insert(std::make_pair(8081, "127.0.0.1"));
    ASSERT_EQ(config::Options::getSocketOptions(), socketOpts);
}