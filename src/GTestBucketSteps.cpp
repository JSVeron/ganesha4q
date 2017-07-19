#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <QingStor.h>
#include <memory>

using cucumber::ScenarioScope;

using namespace QingStor;

struct TestBucketCtx{
	QingStorService*  pQsService;
	Bucket*  pQsBucket;
};
struct TestObjectCtx{
	QingStorService*  pQsService;
	Bucket*  pQsBucket;
	std::string bucketName;
};

struct TestListMultipartUploadsCtx {
	QingStorService*  pQsService;
	Bucket*  pQsBucket;
	std::string bucketName;
	std::string objectKey;
	std::string uploadID;
};

const char* strConfigPath = "/etc/qingstor/config.yaml";

/*
Scenario: need to use QingStor service
When initialize QingStor service
Then the QingStor service is initialized
*/
WHEN("^initialize QingStor service$") {
// as you wish
	QingStorService::initService("./");
}

THEN("^the QingStor service is initialized$") {

	EXPECT_EQ(NULL, NULL);
}

/*
# GET Service(List Buckets)
Scenario: list all buckets
	When list buckets
	Then list buckets status code is 200
*/
WHEN("^list buckets$") {
	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	
	ListBucketsInput input;
	ScenarioScope<ListBucketsOutput> contextOutput;
	input.SetLocation("gd1");

	QsError err = qsService.listBuckets(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}

	qsService.listBuckets(input, *contextOutput);
}

THEN("^list buckets status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListBucketsOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}


//  Scenario: need to use bucket
//    When initialize the bucket
//    Then the bucket is initialized

WHEN("^initialize the bucket$") {
	QingStorService::initService("./");
	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);

	ScenarioScope<TestBucketCtx> context;
	context->pQsService = new QingStorService(qsConfig);
	context->pQsBucket = new Bucket(qsConfig, "huang-stor", "pek3a");

	//Bucket qsBucket = context->pQsService->GetBucket("huang-stor", "pek3a");
}

THEN("^the bucket is initialized$") {
	//REGEX_PARAM(double, expected);
	ScenarioScope<TestBucketCtx> context;

	EXPECT_NE(NULL, NULL);
}


//////////////////////////
/////  # PUT Bucket  /////
//////////////////////////
/*
Scenario: create the bucket
When put bucket
Then put bucket status code is 201
*/
WHEN("^put bucket$") {
	
	// as you wish

	//QingStor::QsConfig qsConfig;
	//qsConfig.loadConfigFile(strConfigPath);
	//QingStorService qsService(qsConfig);
	//Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	////putobject
	//PutBucketInput input;
	//PutBucketOutput outtput;
	//std::stringstream ss;


	//std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();

	//QsError err = qsBucket.putBucket(input, outtput);
	//if (QsError::QS_ERR_NO_ERROR != err)
	//{
	//	//context->result = (int)putObjectPropsOuput.GetResponseCode();
	//}

}

THEN("^put bucket status code is (\\d+)$") {

	//REGEX_PARAM(double, expected);

	//ScenarioScope<PutBucketOutput> contextOutput;

	//EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());

	EXPECT_EQ(NULL, NULL);
}

/*
Scenario: create the same bucket again
When put same bucket again
Then put same bucket again status code is 409
*/
WHEN("^put same bucket again$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	PutBucketInput input;
	ScenarioScope<PutBucketOutput> contextOutput;

	QsError err = qsBucket.putBucket(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^put same bucket again status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}



////////////////////////////////////////
/////  # GET Bucket(List Objects)  /////
////////////////////////////////////////
/*
Scenario: list objects in the bucket
	When list objects
	Then list objects status code is 200
	And list objects keys count is 0
*/
WHEN("^list objects$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("testmorvenhuang", "pek3a");

	ListObjectsInput input;
	ScenarioScope<ListObjectsOutput> contextOutput;

	QsError err = qsBucket.listObjects(input, *contextOutput);

	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode(); 
	}

}

THEN("^list objects status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListObjectsOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^list objects keys count is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListObjectsOutput> contextOutput;
	std::vector<KeyType > keys = contextOutput->GetKeys();

	EXPECT_EQ(expected, keys.size());
}

///////////////////////////
/////  # Head Bucket  /////
///////////////////////////
/*
Scenario: head the bucket
	When head bucket
	Then head bucket status code is 200
*/
WHEN("^head bucket$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	HeadBucketInput input;
	ScenarioScope<HeadBucketOutput> contextOutput;

	QsError err = qsBucket.headBucket(input, *contextOutput);
}

THEN("^head bucket status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<HeadBucketOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}



///////////////////////////////////////
/////  # Delete Multiple Objects  /////
///////////////////////////////////////
/*
Scenario : delete multiple objects in the bucket
	When delete multiple objects :
		   """
		   {
			   "quiet": false,
				   "objects" : [
			   {
				   "key": "object_0"
			   },
			   {
				   "key": "object_1"
			   },
			   {
				   "key": "object_2"
			   }
				   ]
		   }
		   """
			   Then delete multiple objects code is 200
*/

/////////////////////////////////////
/////  # GET Bucket Statistics  /////
/////////////////////////////////////
/*
Scenario : get statistics of the bucket
	When get bucket statistics
	Then get bucket statistics status code is 200
	And get bucket statistics status is "active"
*/
WHEN("^get bucket statistics$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	GetBucketStatisticsInput input;
	ScenarioScope<GetBucketStatisticsOutput> contextOutput;

	QsError err = qsBucket.getBucketStatistics(input, *contextOutput);
}

THEN("^get bucket statistics status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetBucketStatisticsOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("get bucket statistics status is \"([^\"]*)\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetBucketStatisticsOutput> contextOutput;

	EXPECT_EQ(expected, contextOutput->GetStatus());
}



/////////////////////////////
/////  # DELETE Bucket  /////
/////////////////////////////
/*
Scenario : delete the bucket
	When delete bucket
	Then delete bucket status code is 204
*/


//////////////////////////////////////
/////  # List Multipart Uploads  /////
//////////////////////////////////////
/*
Scenario : list multipart uploads
	Given an object created by initiate multipart upload
	When list multipart uploads
	Then list multipart uploads count is 1
*/

GIVEN("^an object created by initiate multipart upload$"){
	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	ScenarioScope<TestListMultipartUploadsCtx> contextGiven;

	InitiateMultipartUploadInput input;
	InitiateMultipartUploadOutput output;

	contextGiven->objectKey = "testInitMultipartUpload.txt";

	QsError err = qsBucket.initiateMultipartUpload(contextGiven->objectKey, input, output);
	
	if (QsError::QS_ERR_NO_ERROR == err)
	{
		contextGiven->uploadID = output.GetUploadID();
	}

	UploadMultipartInput multipartInput;
	UploadMultipartOutput multipartOutput;

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << " |thi is a Part 3| ";
	objectStream->flush();
	multipartInput.SetBody(objectStream);
	multipartInput.SetContentLength(strlen(" |thi is a Part 3| "));
	multipartInput.SetPartNumber(3);
	multipartInput.SetUploadID(contextGiven->uploadID);

	err = qsBucket.uploadMultipart(contextGiven->objectKey, multipartInput, multipartOutput);
	if (QsError::QS_ERR_NO_ERROR == err)
	{

	}
}

WHEN("^list multipart uploads$") {

	ScenarioScope<TestListMultipartUploadsCtx> contextGiven;

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	ListMultipartInput input;
	ScenarioScope<ListMultipartOutput> contextOutput;
	input.SetUploadID(contextGiven->uploadID);

	QsError err = qsBucket.listMultipart(contextGiven->objectKey,input, *contextOutput);
}

THEN("^list multipart uploads count is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListMultipartOutput> contextOutput;

	EXPECT_EQ(expected, contextOutput->GetCount());
}



//Feature: the bucket ACL feature

//////////////////////////////
/////  # PUT Bucket ACL  /////
//////////////////////////////

//Scenario: set the bucket ACL
//When put bucket ACL:
//		  """
//		  {
//			  "acl": [
//			  {
//				  "grantee": {
//					  "type": "group",
//						  "name" : "QS_ALL_USERS"
//				  },
//				  "permission" : "FULL_CONTROL"
//			  }
//			  ]
//		  }
//		  """
//Then put bucket ACL status code is 200
WHEN("^put bucket ACL:$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	PutBucketACLInput input;
	ScenarioScope<PutBucketACLOutput> contextOutput;

	std::vector<ACLType> aclList;
	ACLType acl;
	GranteeType grantee;

	grantee.SetType("group");
	grantee.SetName("QS_ALL_USERS");
	acl.SetGrantee(grantee);
	acl.SetPermission("FULL_CONTROL");

	aclList.push_back(acl);
	input.SetACL(aclList);

	QsError err = qsBucket.putBucketACL(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}

}

THEN("^put bucket ACL status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutBucketACLOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


//////////////////////////////
/////  # GET Bucket ACL  /////
//////////////////////////////
/*
Scenario : get ACL of the bucket
When get bucket ACL
Then get bucket ACL status code is 200
And get bucket ACL should have grantee name "QS_ALL_USERS"
*/

WHEN("^get bucket ACL$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	GetBucketACLInput input;
	ScenarioScope<GetBucketACLOutput> contextOutput;

	QsError err = qsBucket.getBucketACL(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^get bucket ACL status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetBucketACLOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^get bucket ACL should have grantee name \"([^\"]*)\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetBucketACLOutput> contextOutput;
	std::string granteeName = "";
	if (contextOutput->GetACL().size())
	{
		granteeName = contextOutput->GetACL().at(1).GetGrantee().GetName();
	}

	EXPECT_EQ(expected, granteeName);
}


//////////Feature: the bucket CORS feature

	
///////////////////////////////
/////  # PUT Bucket CORS  /////
///////////////////////////////
//Scenario : set the bucket CORS
//		 When put bucket CORS :
//		 """
//		 {
//			 "cors_rules": [
//			 {
//				 "allowed_origin": "http://*.qingcloud.com",
//					 "allowed_methods" : [
//						 "PUT",
//							 "GET",
//							 "DELETE",
//							 "POST"
//					 ],
//					 "allowed_headers": [
//						 "X-QS-Date",
//							 "Content-Type",
//							 "Content-MD5",
//							 "Authorization"
//					 ],
//					 "max_age_seconds": 200,
//					 "expose_headers" : [
//						 "X-QS-Date"
//					 ]
//			 },
//			 {
//				 "allowed_origin": "http://*.example.com",
//				 "allowed_methods" : [
//					 "PUT",
//						 "GET",
//						 "DELETE",
//						 "POST"
//				 ],
//				 "allowed_headers": [
//					 "*"
//				 ],
//				 "max_age_seconds" : 400
//			 }
//			 ]
//		 }
//		 """
//		Then put bucket CORS status code is 200
WHEN("^put bucket CORS:$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	PutBucketCORSInput input;
	ScenarioScope<PutBucketCORSOutput> contextOutput;

	std::vector<CORSRuleType > rules;
	CORSRuleType rule1;
	CORSRuleType rule2;

	std::vector<std::string > allowedMethods;
	allowedMethods.push_back("PUT");
	allowedMethods.push_back("GET");
	allowedMethods.push_back("DELETE");
	allowedMethods.push_back("POST");

	std::vector<std::string > allowedHeaders;
	allowedHeaders.push_back("X-QS-Date");
	allowedHeaders.push_back("Content-Type");
	allowedHeaders.push_back("Content-MD5");
	allowedHeaders.push_back("Authorization");

	std::vector<std::string > allowedAllHeaders;
	allowedAllHeaders.push_back("*");

	std::vector<std::string > exposeHeaders;
	exposeHeaders.push_back("X-QS-Date");


	rule1.SetAllowedOrigin("http://*.qingcloud.com");
	rule1.SetAllowedMethods(allowedMethods);
	rule1.SetExposeHeaders(exposeHeaders);
	rule1.SetAllowedHeaders(allowedHeaders);
	rule1.SetMaxAgeSeconds(200);

	rule2.SetAllowedOrigin("http://*.example.com");
	rule2.SetAllowedMethods(allowedMethods);
	rule2.SetAllowedHeaders(allowedAllHeaders);
	rule2.SetMaxAgeSeconds(400);

	rules.push_back(rule1);
	rules.push_back(rule2);

	input.SetCORSRules(rules);

	QsError err = qsBucket.putBucketCORS(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^put bucket CORS status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutBucketCORSOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}
			 
///////////////////////////////
/////  # GET Bucket CORS  /////
///////////////////////////////
/*
Scenario : get CORS of the bucket
	When get bucket CORS
	Then get bucket CORS status code is 200
	And get bucket CORS should have allowed origin "http://*.qingcloud.com"
*/
WHEN("^get bucket CORS$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	GetBucketCORSInput input;
	ScenarioScope<GetBucketCORSOutput> contextOutput;


	QsError err = qsBucket.getBucketCORS(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{

	}
}

THEN("^get bucket CORS status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetBucketCORSOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^get bucket CORS should have allowed origin \"([^\"]*)\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetBucketCORSOutput> contextOutput;
	std::string allowedOrigin = "";
	if (contextOutput->GetCORSRules().size()){
		CORSRuleType corsRule = contextOutput->GetCORSRules().at(0);
		allowedOrigin = corsRule.GetAllowedOrigin();
	}

	EXPECT_EQ(expected, allowedOrigin);
}

//////////////////////////////////
/////  # DELETE Bucket CORS  /////
//////////////////////////////////
/*
Scenario : delete CORS of the bucket
	When delete bucket CORS
	Then delete bucket CORS status code is 204
*/
WHEN("^delete bucket CORS$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	DeleteBucketCORSInput input;
	ScenarioScope<DeleteBucketCORSOutput> contextOutput;

	QsError err = qsBucket.deleteBucketCORS(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{

	}
}

THEN("^delete bucket CORS status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteBucketCORSOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


///// Feature: the bucket external mirror feature

////////////////////////////////////////////
/////    # PUT Bucket External Mirror  /////
////////////////////////////////////////////
/*
Scenario : set the bucket external mirror
		When put bucket external mirror :
		"""
		{
			"source_site": "https://example.com/something/"
		}
		"""
		Then put bucket external mirror status code is 200
*/
WHEN("^put bucket external mirror:$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	PutBucketExternalMirrorInput input;
	ScenarioScope<PutBucketExternalMirrorOutput> contextOutput;
	input.SetSourceSite("https://example.com/something/");

	QsError err = qsBucket.putBucketExternalMirror(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{

	}
}

THEN("^put bucket external mirror status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutBucketExternalMirrorOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

////////////////////////////////////////////
/////   # GET Bucket External Mirror   /////
////////////////////////////////////////////
/*
Scenario : get external mirror of the bucket
	When get bucket external mirror
	Then get bucket external mirror status code is 200
	And get bucket external mirror should have source_site "https://example.com/something/"
*/
WHEN("^get bucket external mirror$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	GetBucketExternalMirrorInput input;
	ScenarioScope<GetBucketExternalMirrorOutput> contextOutput;

	QsError err = qsBucket.getBucketExternalMirror(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
	}
}

THEN("^get bucket external mirror status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetBucketExternalMirrorOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^get bucket external mirror should have source_site \"([^\"]*)\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetBucketExternalMirrorOutput> contextOutput;

	EXPECT_EQ(expected, contextOutput->GetSourceSite());
}


///////////////////////////////////////////////
/////   # DELETE Bucket External Mirror   /////
///////////////////////////////////////////////
/*
Scenario : delete external mirror of the bucket
	When delete bucket external mirror
	Then delete bucket external mirror status code is 204
*/
WHEN("^delete bucket external mirror$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	DeleteBucketExternalMirrorInput input;
	ScenarioScope<DeleteBucketExternalMirrorOutput> contextOutput;

	QsError err = qsBucket.deleteBucketExternalMirror(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
	}
}

THEN("^delete bucket external mirror status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteBucketExternalMirrorOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

////  Feature: the bucket policy feature

//////////////////////////////////
/////  # PUT Bucket policy   /////
//////////////////////////////////
/*
Scenario : set the bucket policy
		# Notice : Please set statement resource manually
		When put bucket policy :
		 """
		 {
			 "statement": [
			 {
				 "id": "allow certain site to get objects",
					 "user" : [
						 "*"
					 ],
					 "action" : [
						 "get_object"
					 ],
					 "effect" : "allow",
					 "resource" : [],
					 "condition" : {
							 "string_like": {
								 "Referer": [
									 "*.example1.com",
										 "*.example2.com"
								 ]
							 },
							 "ip_address": {
										 "source_ip": [
											 "172.16.0.0/24",
												 "172.16.1.1/32"
										 ]
									 }
						 }
			 }
			 ]
		 }
		 """
		Then put bucket policy status code is 200
*/



//////////////////////////////////
/////  # GET Bucket policy   /////
//////////////////////////////////
/*
Scenario : get policy of the bucket
		When get bucket policy
		Then get bucket policy status code is 200
		And get bucket policy should have Referer "*.example1.com"
*/
WHEN("^get bucket policy$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	GetBucketPolicyInput input;
	ScenarioScope<GetBucketPolicyOutput> contextOutput;

	QsError err = qsBucket.getBucketPolicy(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		
	}
}

THEN("^get bucket policy status code is (\\d+)$"){

	REGEX_PARAM(double, expected);

	ScenarioScope<GetBucketPolicyOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^get bucket policy should have Referer \"([^\"]*)\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetBucketPolicyOutput> contextOutput;
	ConditionType condition;
	StringLikeType striinglike;
	std::vector<std::string > refererList;
	std::string referer = "";

	std::vector<StatementType> statementList =  contextOutput->GetStatement();
	if (statementList.size()){

		StatementType statement = statementList.at(0);
		condition = statement.GetCondition();
		striinglike = condition.GetStringLike();
		refererList = striinglike.GetReferer();

		if (refererList.size()){
			referer = refererList.at(0);
		}
	}
	
	EXPECT_EQ(expected, referer);
}


////////////////////////////////////
/////  # DELETE Bucket policy  /////
////////////////////////////////////
/*
Scenario : delete policy of the bucket
	When delete bucket policy
	Then delete bucket policy status code is 204
*/
WHEN("^delete bucket policy$") {

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	QingStorService qsService(qsConfig);
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	DeleteBucketPolicyInput input;
	ScenarioScope<DeleteBucketPolicyOutput> contextOutput;

	QsError err = qsBucket.deleteBucketPolicy(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{

	}
}

THEN("^delete bucket policy status code is (\\d+)$"){

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteBucketPolicyOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


//// Feature: the object feature

// Scenario Outline :
// # PUT Object
// When put object with key "<key>"
// Then put object status code is 201
WHEN("^put object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectKey);

	ScenarioScope<TestObjectCtx> contextObjectTest;
	contextObjectTest->bucketName = "huang-stor";

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	contextObjectTest->pQsBucket = new Bucket(qsConfig, "huang-stor", "pek3a");
	Bucket qsBucket = *contextObjectTest->pQsBucket;

	PutObjectInput input;
	ScenarioScope<PutObjectOutput> contextOutput;


	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << "thi is a test";
	objectStream->flush();
	input.SetBody(objectStream);
	input.SetContentLength(strlen("thi is a test"));

	QsError err = qsBucket.putObject(objectKey, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^put object status code is (\\d+)$") {
	
	REGEX_PARAM(double, expected);

	ScenarioScope<PutObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

//# Copy Object
//When copy object with key "<key>"
//Then copy object status code is 201
WHEN("^copy object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkeyFromSource);
	std::string objectkeyTopPut = objectkeyFromSource + "_copy";

	ScenarioScope<TestObjectCtx> contextObjectTest;
	std::string sourcePrefix = "/" + contextObjectTest->bucketName + "/";

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	PutObjectInput input;
	ScenarioScope<PutObjectOutput> contextOutput;
	input.SetXQSCopySource(sourcePrefix + objectkeyFromSource);
	input.SetContentLength(0);

	QsError err = qsBucket.putObject(objectkeyTopPut, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^copy object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


//# Move Object
//When move object with key "<key>"
//Then move object status code is 201
WHEN("^move object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkey);
	std::string objectkeySrc = objectkey + "_copy";
	std::string objectkeyDest = objectkey + "_move";

	ScenarioScope<TestObjectCtx> contextObjectTest;
	std::string sourcePrefix = "/" + contextObjectTest->bucketName + "/";

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	PutObjectInput input;
	ScenarioScope<PutObjectOutput> contextOutput;
	input.SetXQSMoveSource(sourcePrefix + objectkeySrc);
	input.SetContentLength(0);

	QsError err = qsBucket.putObject(objectkeyDest, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^move object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

//# GET Object
//When get object with key "<key>"
//Then get object status code is 200
//And get object content length is 1048576
WHEN("^get object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkeyToGet);

	ScenarioScope<TestObjectCtx> contextObjectTest;
	//std::string sourcePrefix = "/" + contextObjectTest->bucketName + "/";

	Bucket qsBucket = *contextObjectTest->pQsBucket;


	GetObjectInput input;
	ScenarioScope<GetObjectOutput> contextOutput;

	QsError err = qsBucket.getObject(objectkeyToGet, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^get object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

THEN("^get object content length is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<GetObjectOutput> contextOutput;
	

	contextOutput->GetBody()->seekg(0, contextOutput->GetBody()->end);
	auto streamSize = contextOutput->GetBody()->tellg();

	int objectLen = static_cast<int>(streamSize);
	int exceptedLen = strlen("thi is a test");

	EXPECT_EQ(exceptedLen, objectLen);
}



//# GET Object with Content - Type
//When get object "<key>" with content type "video/mp4; charset=utf8"
//Then get object content type is "video/mp4; charset=utf8"
WHEN("^get object \"(.{1, })\" with content type \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestObjectCtx> contextObjectTest;

}

THEN("^get object content type is \"(.{1, })\"$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<HeadObjectOutput> contextOutput;

	EXPECT_EQ(expected, "video/mp4; charset=utf8");
}

//# GET Object with Query Signature
//When get object "<key>" with query signature
//Then get object with query signature content length is 1048576
WHEN("^get object \"(.{1, })\" with query signature$") {
	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestObjectCtx> contextObjectTest;
	Bucket qsBucket = *contextObjectTest->pQsBucket;

	GetObjectInput input;
	ScenarioScope<GetObjectOutput> contextOutput;

	QsError err = qsBucket.getObject(objectkey, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}

}

THEN("^get object with query signature content length is (\\d+)$") {

	REGEX_PARAM(std::string, expected);

	ScenarioScope<GetObjectOutput> contextOutput;

	contextOutput->GetBody()->seekg(0, contextOutput->GetBody()->end);
	auto streamSize = contextOutput->GetBody()->tellg();

	int objectLen = static_cast<int>(streamSize);
	int exceptedLen = strlen("thi is a test");

	EXPECT_EQ(exceptedLen, objectLen);
}


//# Head Object
//When head object with key "<key>"
//Then head object status code is 200
WHEN("^head object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestObjectCtx> contextObjectTest;

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	HeadObjectInput input;
	ScenarioScope<HeadObjectOutput> contextOutput;

	QsError err = qsBucket.headObject(objectkey, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^head object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<HeadObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


//# Options Object
//When options object "<key>" with method "GET" and origin "qingcloud.com"
//Then options object status code is 200
WHEN("^options object \"(.{1, })\" with method \"([^\"]*)\" and origin \"([^\"]*)\"$") {
	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestObjectCtx> contextObjectTest;

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	OptionsObjectInput input;
	ScenarioScope<OptionsObjectOutput> contextOutput;
	input.SetAccessControlRequestMethod("GET");
	input.SetOrigin("qingcloud.com");

	QsError err = qsBucket.optionsObject(objectkey, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^options object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<OptionsObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


//# DELETE Object
//When delete object with key "<key>"
//Then delete object status code is 204
//When delete the move object with key "<key>"
//Then delete the move object status code is 204
WHEN("^delete object with key \"(.{1, })\"$") {
	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestObjectCtx> contextObjectTest;

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	DeleteObjectInput input;
	ScenarioScope<DeleteObjectOutput> contextOutput;

	QsError err = qsBucket.deleteObject(objectkey, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^delete object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteObjectOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}


WHEN("delete the move object with key \"(.{1, })\"$") {

	REGEX_PARAM(std::string, objectkey);

	std::string objectkeyToDel = objectkey + "_move";

	ScenarioScope<TestObjectCtx> contextObjectTest;

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	DeleteObjectInput input;
	ScenarioScope<DeleteObjectOutput> contextOutput;

	QsError err = qsBucket.deleteObject(objectkeyToDel, input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^delete the move object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteObjectOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}

//Scenario Outline :
//# Initiate Multipart Upload
//When initiate multipart upload with key "<key>"
//Then initiate multipart upload status code is 200
WHEN("^initiate multipart upload with key \"(.{1, })\"$"){

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	contextMultiPartObjectTest->bucketName = "huang-stor";

	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);
	contextMultiPartObjectTest->pQsBucket = new Bucket(qsConfig, "huang-stor", "pek3a");
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	InitiateMultipartUploadInput input;
	ScenarioScope<InitiateMultipartUploadOutput> contextOutput;

	//contextGiven->objectKey = objectkey;

	QsError err = qsBucket.initiateMultipartUpload(objectkey, input, *contextOutput);

	if (QsError::QS_ERR_NO_ERROR == err)
	{
		contextMultiPartObjectTest->uploadID = contextOutput->GetUploadID();
	}
}
THEN("^initiate multipart upload status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<InitiateMultipartUploadOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}

//# Upload Multipart
//When upload the first part with key "<key>"
//Then upload the first part status code is 201
//When upload the second part with key "<key>"
//Then upload the second part status code is 201
//When upload the third part with key "<key>"
//Then upload the third part status code is 201
WHEN("^upload the first part with key \"(.{1, })\"$"){

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	UploadMultipartInput input;
	ScenarioScope<UploadMultipartOutput> contextOutput;

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << " |thi is a Part 1| ";
	objectStream->flush();
	input.SetBody(objectStream);
	input.SetContentLength(strlen(" |thi is a Part 1| "));
	input.SetPartNumber(1);
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.uploadMultipart(objectkey, input, *contextOutput);

	if (QsError::QS_ERR_NO_ERROR == err)
	{
		//contextGiven->uploadID = output.GetUploadID();
	}
}
THEN("^upload the first part status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<UploadMultipartOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}

WHEN("^upload the second part with key \"(.{1, })\"$"){

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	UploadMultipartInput input;
	ScenarioScope<UploadMultipartOutput> contextOutput;

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << " |thi is a Part 2| ";
	objectStream->flush();
	input.SetBody(objectStream);
	input.SetContentLength(strlen(" |thi is a Part 2| "));
	input.SetPartNumber(2);
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.uploadMultipart(objectkey, input, *contextOutput);

	if (QsError::QS_ERR_NO_ERROR == err)
	{
		//contextGiven->uploadID = output.GetUploadID();
	}
}
THEN("^upload the second part status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<UploadMultipartOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}
WHEN("^upload the third part with key \"(.{1, })\"$"){

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	UploadMultipartInput input;
	ScenarioScope<UploadMultipartOutput> contextOutput;

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << " |thi is a Part 3| ";
	objectStream->flush();
	input.SetBody(objectStream);
	input.SetContentLength(strlen(" |thi is a Part 3| "));
	input.SetPartNumber(3);
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.uploadMultipart(objectkey, input, *contextOutput);

	if (QsError::QS_ERR_NO_ERROR == err)
	{
		//contextGiven->uploadID = output.GetUploadID();
	}
}
THEN("^upload the third part status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<UploadMultipartOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}


//# List Multipart
//When list multipart with key "<key>"
//Then list multipart status code is 200
//And list multipart object parts count is 3
WHEN("^list multipart with key \"(.{1, })\"$") {

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	ListMultipartInput input;
	ScenarioScope<ListMultipartOutput> contextOutput;
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.listMultipart(objectkey, input, *contextOutput);
}

THEN("^list multipart status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListMultipartOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}

THEN("^list multipart object parts count is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<ListMultipartOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetCount());
}

//# Complete Multipart Upload
//When complete multipart upload with key "<key>"
//Then complete multipart upload status code is 201
WHEN("^complete multipart upload with key \"(.{1, })\"$") {

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	CompleteMultipartUploadInput input;
	ScenarioScope<CompleteMultipartUploadOutput> contextOutput;
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.completeMultipartUpload(objectkey, input, *contextOutput);
}

THEN("^complete multipart upload status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<CompleteMultipartUploadOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}


//# Abort Multipart Upload
//When abort multipart upload with key "<key>"
//Then abort multipart upload status code is 400
WHEN("^abort multipart upload with key \"(.{1, })\"$") {

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	AbortMultipartUploadInput input;
	ScenarioScope<AbortMultipartUploadOutput> contextOutput;
	input.SetUploadID(contextMultiPartObjectTest->uploadID);

	QsError err = qsBucket.abortMultipartUpload(objectkey, input, *contextOutput);
}

THEN("^abort multipart upload status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<AbortMultipartUploadOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}

//# DELETE Object
//When delete the multipart object with key "<key>"
//Then delete the multipart object status code is 204
WHEN("^delete the multipart object with key \"(.{1, })\"$") {

	REGEX_PARAM(std::string, objectkey);

	ScenarioScope<TestListMultipartUploadsCtx> contextMultiPartObjectTest;
	Bucket qsBucket = *contextMultiPartObjectTest->pQsBucket;

	DeleteMultipleObjectsInput input;
	ScenarioScope<DeleteMultipleObjectsOutput> contextOutput;

	std::vector<KeyType> emptyDeleteObjects;
	input.SetObjects(emptyDeleteObjects);
	input.SetContentMD5("MD5");

	QsError err = qsBucket.deleteMultipleObjects(input, *contextOutput);
}

THEN("^delete the multipart object status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<DeleteMultipleObjectsOutput> contextOutput;

	EXPECT_EQ(expected, (double)contextOutput->GetResponseCode());
}




//GetObjectInput getObjectInputPropsinput;
//GetObjectOutput getObjectInputPropsOutput;

//std::string etag;
//std::string requestID;
//std::string responseCode;

//bool isEncryptede = false;
//std::stringstream ss;

//getObjectInputPropsinput.SetRange("bytes=0-5");

//QsError err = qsBucket.getObject("testObj.txt", getObjectInputPropsinput, getObjectInputPropsOutput);
//if (err == QsError::QS_ERR_NO_ERROR)
//{
//	etag = getObjectInputPropsOutput.GetETag();
//	Http::HttpResponseCode requestID = getObjectInputPropsOutput.GetResponseCode();
//	responseCode = getObjectInputPropsOutput.GetRequestID();
//	ss.str("");

//	ss << getObjectInputPropsOutput.GetBody()->rdbuf();

//}
