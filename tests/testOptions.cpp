#include "config/Options.hpp"

#include <gtest/gtest.h>

class testOptionsBasic : public ::testing::Test {
    protected:

        void SetUp(void) {
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::Block>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::Listen>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::End>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::Block>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::Route>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::End>({}));
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::End>({}));
        }

        void TearDown(void) {
            for (auto& directive : m_directives) { delete directive; }
        }

        std::vector<config::DirectiveTypeTraitsBase*> m_directives;
};

TEST_F(testOptionsBasic, testGetNextBlock) {
    std::vector<config::DirectiveTypeTraitsBase*> cmp;

    cmp = std::vector<config::DirectiveTypeTraitsBase*>(
        m_directives.begin(), m_directives.begin() + 3);
    ASSERT_EQ(config::getNextBlock(m_directives), cmp);

    cmp = std::vector<config::DirectiveTypeTraitsBase*>(
        m_directives.begin() + 3, m_directives.end());
    ASSERT_EQ(config::getNextBlock(), cmp);

    ASSERT_TRUE(config::getNextBlock().empty());
}

TEST_F(testOptionsBasic, testGetNextBlockUnclosed) {
    ASSERT_THROW(
        config::getNextBlock(std::vector<config::DirectiveTypeTraitsBase*>(
            m_directives.begin(), m_directives.begin() + 2)),
        std::exception);

    ASSERT_THROW(
        config::getNextBlock(std::vector<config::DirectiveTypeTraitsBase*>(
            m_directives.begin() + 3, m_directives.begin() + 6)),
        std::exception);
}

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
            m_opts = config::parse("/tmp/testsOptions.tmp");
        }

        ~testOptionsExtreme(void) { system("rm /tmp/testsOptions.tmp"); }

        std::vector< smt::shared_ptr<config::Opts> > m_opts;
};

TEST_F(testOptionsExtreme, testFirstRoute) {

    smt::shared_ptr<config::Opts> opt(new config::Opts);
    opt->m_target = "/";
    opt->m_host = "localhost";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = true;
    opt->m_cgi_extension = "";

    ASSERT_EQ(*(m_opts[0]), *opt);
}

TEST_F(testOptionsExtreme, testSecondRoute) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/python/";
    opt->m_host = "localhost";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/python/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET"});
    opt->m_index = "./websites/index.html";
    opt->m_autoindex = false;
    opt->m_cgi_extension = ".py";
    ASSERT_EQ(*(m_opts[2]), *opt);
}

TEST_F(testOptionsExtreme, testThirdRoute) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/php/";
    opt->m_host = "localhost";
    opt->m_port = "8080";
    opt->m_root = "./websites/cgi/php/";
    opt->m_server_name = "";
    opt->m_error_pages = std::map<int, ft::file>();
    opt->m_max_body_size = 1000000;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = false;
    opt->m_cgi_extension = ".php";
    ASSERT_EQ(*(m_opts[1]), *opt);
}

TEST_F(testOptionsExtreme, testSecondBlock) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/";
    opt->m_host = "localhost";
    opt->m_port = "8081";
    opt->m_root = "./websites/";
    opt->m_server_name = "domain.com";
    opt->m_error_pages[404] = "./websites/errors/404.html";
    opt->m_max_body_size = 80;
    opt->m_allowed_methods = std::vector<std::string>({"GET", "POST"});
    opt->m_index = "";
    opt->m_autoindex = false;
    opt->m_cgi_extension = "";
    ASSERT_EQ(*(m_opts[3]), *opt);
}

TEST_F(testOptionsExtreme, testThirdBlock) {
    smt::shared_ptr<config::Opts> opt(new config::Opts);

    opt->m_target = "/";
    opt->m_host = "localhost";
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
    ASSERT_EQ(*(m_opts[4]), *opt);
}

TEST_F(testOptionsExtreme, testGetOptionsByHostAndPort) {
    config::Options::getInstance(m_opts);

    config::Options::getOpts("8080", "localhost", "/", "");

    config::Options::getOpts("8080", "localhost", "/", "great");

    config::Options::getOpts("8081", "localhost", "/", "domain.com");

    config::Options::getOpts("8081", "localhost", "/", "domain.net");

    config::Options::getOpts("8081", "localhost", "/", "domain.pt");

    config::Options::getOpts("8081", "localhost", "/", "");
}