#include "../include/server.h"
#include "../include/main.h"
#include <criterion/criterion.h>
#include <criterion/logging.h>

Test(findCharacterIndex_function , simple_defined) {
    cr_log_warn("findCharacterIndex - Function | testing existing function");
    int result = findCharacterIndex("Hello world" , 'H');
}

Test(findCharacterIndex_test_1 , simple_test) {
    cr_log_warn("findCharacterIndex - Function | simple test to retreive H in Hello World (result = 1)");
    int result = findCharacterIndex("Hello world" , 'H');
    cr_assert_eq(1 , (result + 1));
}

Test(findCharacterIndex_test_2 , simple_test) {
    cr_log_warn("findCharacterIndex - Function | simple test to retreive G in Hello Worl (result = -1)");
    int result = findCharacterIndex("Hello world" , 'G');
    cr_assert_eq(-1 , result);
}

Test(getMimeType_function , simple_defined) {
    cr_log_warn("getMimeType - Function | testing existing function");
    getMimeType(".html");
}

Test(getMimeType_test_1 , simple_test) {
    cr_log_warn("getMimeType - Fucntion | simple test with var = test (result = NULL)");
    char *result;
    result = getMimeType("test");
    cr_assert_eq(result , NULL);
}

Test(getMimeType_test_2 , simple_test) {
    cr_log_warn("getMimeType - Fucntion | simple test with var = test.html and test.htm(result = text/html)");
    char *result;
    result = getMimeType("test.html");
    cr_assert_eq(0 , strcmp("text/html" , result));
    result = getMimeType("test.htm");
    cr_assert_eq(0 , strcmp("text/html" , result));
}

Test(getMimeType_test_3 , simple_test) {
    cr_log_warn("getMimeType - Fucntion | simple test with var = test.test (result = text/plain)");
    char *result;
    result = getMimeType("test.test");
    cr_assert_eq(0 , strcmp("text/plain" , result));
}

Test(getMimeType_test_4 , simple_test) {
    cr_log_warn("getMimeType - Fucntion | simple test with var = test.jpg and test.jpeg (result = image/jpeg)");
    char *result;
    result = getMimeType("test.jpg");
    cr_assert_eq(0 , strcmp("image/jpeg" , result));
    result = getMimeType("test.jpeg");
    cr_assert_eq(0 , strcmp("image/jpeg" , result));
}

Test(getMimeType_test_5 , simple_test) {
    cr_log_warn("getMimeType - Fucntion | simple test with var = test.png (result = image/png)");
    char *result;
    result = getMimeType("test.png");
    cr_assert_eq(0 , strcmp("image/png" , result));
}

Test(generateResponseStatusLine_function , simple_defined) {
    cr_log_warn("generateResponseStatusLine - Function | testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_OK , responseStream);
    fclose(responseStream);
}

Test(generateResponseStatusLine_test_1, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 200");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_OK , responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 200 OK\r\n", response));
}

Test(generateResponseStatusLine_test_2, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 201");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_CREATED, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 201 CREATED\r\n", response));
}


Test(generateResponseStatusLine_test_3, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 204");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_NO_CONTENT, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 204 NO CONTENT\r\n", response));
}


Test(generateResponseStatusLine_test_4, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 400");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_BAD_REQUEST, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 400 BAD REQUEST\r\n", response));
}


Test(generateResponseStatusLine_test_5, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 403");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_FORBIDDEN, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 403 FORBIDDEN\r\n", response));
}
Test(generateResponseStatusLine_test_6, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 404");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_NOT_FOUND, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 404 NOT FOUND\r\n", response));
}

Test(generateResponseStatusLine_test_7, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 409");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_CONFLICT, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 409 CONFLICT\r\n", response));
}

Test(generateResponseStatusLine_test_8, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 501");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_NOT_IMPLEMENTED, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 501 NOT IMPLEMENTED\r\n", response));
}

Test(generateResponseStatusLine_test_9, simple_test) {
    cr_log_warn("generateResponseStatusLine - Function |  simple test with HTTP RESPONSE 505");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseStatusLine(response , HTTP_STATUS_NOT_SUPPORTED, responseStream);
    fclose(responseStream);
    cr_assert_eq(0 , strcmp("HTTP/1.1 505 NOT SUPPORTED\r\n", response));
}

Test(generateResponseHeaders_function , simple_defined) {
    cr_log_warn("generateResponseHeaders - Function | testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponseHeaders(response , "test" , 10 , responseStream , "test");
    fclose(responseStream);
}

Test(generateResponse_function , simple_defined) {
    cr_log_warn("generateResponse - Function | testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateResponse(response , 200 , NULL , responseStream , "text/plain" , 1 , "ok");;
    fclose(responseStream);
}

Test(getFile_function , simple_defined) {
    cr_log_warn("getFile - Function | testing existing function");
    char *response = NULL; 
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    getFile(responseStream , response , "/etc/passwd" , 1);
    fclose(responseStream);
}

Test(getFile_test_1 , simple_test) {
    cr_log_warn("getFile - Function | simple test with existing file /etc/passwd");
    char *response = NULL; 
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    int result = getFile(responseStream , response , "/etc/passwd" , 1);
    fclose(responseStream);
    cr_assert_eq(0 , result);
}

Test(getFile_test_2 , simple_test) {
    cr_log_warn("getFile - Function | simple test with not existing file");
    char *response = NULL; 
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    int result = getFile(responseStream , response , "/text.txt" , 1);
    fclose(responseStream);
    cr_assert_eq(-1 , result);
}

Test(getDirectory_function , simple_defined) {
    cr_log_warn("getDirectory - Function | testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    struct stat statbuf;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    getDirectory(responseStream , response , "/etc/" , statbuf , 1);
    fclose(responseStream);
}

Test(getDirectory_test_1 , simple_test) {
    cr_log_warn("getDirectory - Function | simple test with existing folder");
    char *response = NULL;
    size_t buffersize = 0;
    struct stat statbuf;
    FILE *responseStream = open_memstream(&response , &buffersize);
    int result = getDirectory(responseStream , response , "/etc/" , statbuf , 1);
    fclose(responseStream);
    cr_assert_eq(0 , result);
}

Test(getDirectory_test_2 , simple_test) {
    cr_log_warn("getDirectory - Function | simple test with not existing folder without '/'");
    char *response = NULL;
    size_t buffersize = 0;
    struct stat statbuf;
    FILE *responseStream = open_memstream(&response , &buffersize);
    int result = getDirectory(responseStream , response , "/testtest" , statbuf , 1);
    fclose(responseStream);
    cr_assert_eq(-2 , result);
}

Test(trimStart_function , simple_defined) {
    cr_log_warn("trimStart - Function | Testing existing function");
    trimStart("test");
}

Test(trimStart_test_1 , simple_test) {
    cr_log_warn("trimStart - Function | simple test with '    test   '");
    char *result;
    result = trimStart("    test   ");
    cr_assert_eq(0 , strcmp("test   " , result));
}

Test(addFile_function , simple_defined) {
    cr_log_warn("addFile - Function | Testing existing function");
    
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    addFile(responseStream , response , "./test.txt" , 1 , "test" , 1);
    fclose(responseStream);
}

Test(deleteFile_function , simple_defined) {
    cr_log_warn("deleteFile - Function | Testing existing function");
    
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    deleteFile(responseStream , response , "./test.txt" , 1);
    fclose(responseStream);
}

Test(generateInfo_function , simple_defined) {
    cr_log_warn("generateInfo - Function | Testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateInfo(responseStream , response , 1);
    fclose(responseStream);
}

Test(generateSwagger_function , simple_defined) {
    cr_log_warn("generateSwagger - Function | Testing existing function");
    char *response = NULL;
    size_t bufferSize = 0;
    FILE *responseStream = open_memstream(&response , &bufferSize);
    generateSwagger(responseStream , response , 1);
    fclose(responseStream);
}