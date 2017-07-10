#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <Calculator.h>
#include <service/QingStor.h>
#include <service/Bucket.h>

using cucumber::ScenarioScope;

struct TestBucketCtx {
    QingStorService bucket;
    double result;
};

//  Scenario: need to use bucket
//    When initialize the bucket
//    Then the bucket is initialized

WHEN("^initialize the bucket") {
    QingStor::QsConfig qsConfig("C:/Users/Morven.Huang/Dropbox/sdk-C++/src/Debug/config.yaml");

    QingStorService qsService(qsConfig);

    ScenarioScope<Bucket> qsBucket;

    qsBucket = qsService.GetBucket("huang-stor","pek3a");
}

THEN("^the bucket is initialized") {
    REGEX_PARAM(double, expected);
    ScenarioScope<CalcCtx> context;

    EXPECT_EQ(expected, context->result);
}


//////////////////////////
/////  # PUT Bucket  /////
//////////////////////////
 /*
  Scenario: create the bucket
    When put bucket
    Then put bucket status code is 201
*/
WHEN("^put bucket") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.add();
}

THEN("^put bucket status code is 201$") {
 
    ScenarioScope<CalcCtx> context;

    EXPECT_EQ(201, context->result);
}
/*
  Scenario: create the same bucket again
    When put same bucket again
    Then put same bucket again status code is 409
*/
///////////////
GIVEN("^I have entered (\\d+) into the calculator$") {
    REGEX_PARAM(double, n);
    ScenarioScope<TestBucketCtx> context;

    context->calc.push(n);
}

WHEN("^I press add") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.add();
}

WHEN("^I press divide") {
    ScenarioScope<CalcCtx> context;

    context->result = context->calc.divide();
}

THEN("^the result should be (.*) on the screen$") {
    REGEX_PARAM(double, expected);
    ScenarioScope<CalcCtx> context;

    EXPECT_EQ(expected, context->result);
}

