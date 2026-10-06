#include "detective.h"
#include <gtest/gtest.h>
#include <memory>

#include "exceptions.h"

class DetectiveTest : public ::testing::Test {
protected:
    void SetUp() override {
        detective = std::make_shared<Detective>();
        medicine = std::make_shared<Medicine>(4, 4);
    }

    std::shared_ptr<Detective> detective;
    std::shared_ptr<Medicine> medicine;
};

TEST_F(DetectiveTest, ConstructorDefault) {
    EXPECT_FALSE(detective->is_dead());
    EXPECT_FALSE(detective->is_moving());
}

TEST_F(DetectiveTest, ConstructorWithParameters) {
    auto customDetective = std::make_shared<Detective>(100, 100, 10, 1.0f, 20, 1.0f, 15);
    EXPECT_FALSE(customDetective->is_dead());
}

TEST_F(DetectiveTest, TreatMethod) {
    EXPECT_THROW(detective->treat(medicine), ObjectParameterException);

}

TEST_F(DetectiveTest, Inheritance) {
    EXPECT_FALSE(detective->is_dead());
    detective->cause_damage(5);
    EXPECT_FALSE(detective->is_dead());
}