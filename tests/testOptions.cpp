#include "config/Options.hpp"

#include <gtest/gtest.h>

class testOptions : public ::testing::Test {
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

        std::vector<config::DirectiveTypeTraitsBase*> m_directives;
};

TEST_F(testOptions, testGetNextBlock) {
    std::vector<config::DirectiveTypeTraitsBase*> cmp;

    cmp = std::vector<config::DirectiveTypeTraitsBase*>(
        m_directives.begin(), m_directives.begin() + 3);
    ASSERT_EQ(config::getNextBlock(m_directives), cmp);

    cmp = std::vector<config::DirectiveTypeTraitsBase*>(
        m_directives.begin() + 3, m_directives.end());
    ASSERT_EQ(config::getNextBlock(), cmp);

    ASSERT_TRUE(config::getNextBlock().empty());
}

TEST_F(testOptions, testGetNextBlockUnclosed) {
    ASSERT_THROW(
        config::getNextBlock(std::vector<config::DirectiveTypeTraitsBase*>(
            m_directives.begin(), m_directives.begin() + 2)),
        std::exception);

    ASSERT_THROW(
        config::getNextBlock(std::vector<config::DirectiveTypeTraitsBase*>(
            m_directives.begin() + 3, m_directives.begin() + 6)),
        std::exception);
}