#include "config/Options.hpp"

#include <gtest/gtest.h>

class testOptions : public ::testing::Test {
    protected:

        void SetUp(void) {
            m_directives.push_back(
                new config::DirectiveTypeTraits<config::Block>(
                    {"server", "{"}));
        }

        std::vector<config::DirectiveTypeTraitsBase*> m_directives;
};

TEST_F(testOptions, testGetNextBlock) {
    // create a vector of directivetypetraits
}