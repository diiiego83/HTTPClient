#include "../googletest/googletest/include/gtest/gtest.h"
#include "../../src/HTTPClient.h"

using namespace httpclient;

TEST(utx_http_client, get) {
  HTTPClient http;
  http.get("https://httpstat.us/200");
  ASSERT_FALSE(http.get_response_code()-200);
  ASSERT_EQ(http.get_resonse_message(), "200 OK");
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}