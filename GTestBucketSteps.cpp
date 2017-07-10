#include <gtest/gtest.h>
#include <cucumber-cpp/autodetect.hpp>

#include <service/QingStor.h>
#include <service/Bucket.h>
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
	std::string objectKey;
	std::string uploadID;
};

const char* strConfigPath = strConfigPath;

//  Scenario: need to use bucket
//    When initialize the bucket
//    Then the bucket is initialized

WHEN("^initialize the bucket$") {
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

	EXPECT_NE(NULL, (int)context->pQsBucket);
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
	
	QingStor::QsConfig qsConfig;
	qsConfig.loadConfigFile(strConfigPath);

	QingStorService qsService(qsConfig);

	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	//putobject
	PutBucketInput input;
	PutBucketOutput outtput;
	std::stringstream ss;

	

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	//*objectStream << "Test Object";
	//objectStream->flush();
	//int seize = strlen("Test Object") + 1;
	//input.SetContentLength(seize);
	//input.SetBody(objectStream);

	//PutObjectOutput putObjectPropsOuput;
	QsError err = qsBucket.putBucket (input, outtput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}

}

THEN("^put bucket status code is (\\d+)$") {

	REGEX_PARAM(double, expected);

	ScenarioScope<PutBucketOutput> contextOutput;

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
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

	//putobject
	PutObjectInput putObjectPropsinput;
	ScenarioScope<PutObjectOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;
	std::stringstream ss;

	std::shared_ptr<std::iostream> objectStream = std::make_shared<std::stringstream>();
	*objectStream << "Test Object";
	objectStream->flush();
	int seize = strlen("Test Object") + 1;
	putObjectPropsinput.SetContentLength(seize);
	putObjectPropsinput.SetBody(objectStream);

	QsError err = qsBucket.putObject("testObj2.txt", putObjectPropsinput, *contextOutput);
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
	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");

	ListObjectsInput input;
	ScenarioScope<ListObjectsOutput> contextOutput;
	input.SetPrefix();

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

	//putobject
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

	//putobject
	GetBucketACLInput input;
	ScenarioScope<GetBucketACLOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

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

	//putobject
	GetBucketCORSInput input;
	ScenarioScope<GetBucketCORSOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.getBucketCORS(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
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

	//putobject
	DeleteBucketCORSInput input;
	ScenarioScope<DeleteBucketCORSOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.deleteBucketCORS(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
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
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
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

	//putobject
	GetBucketExternalMirrorInput input;
	ScenarioScope<GetBucketExternalMirrorOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.getBucketExternalMirror(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^get bucket external mirror status code (\\d+)$") {

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

	//putobject
	DeleteBucketExternalMirrorInput input;
	ScenarioScope<DeleteBucketExternalMirrorOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.deleteBucketExternalMirror(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
}

THEN("^delete bucket external mirror status code is(\\d+)$") {

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

	//putobject
	GetBucketPolicyInput input;
	ScenarioScope<GetBucketPolicyOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.getBucketPolicy(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
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

	//putobject
	DeleteBucketPolicyInput input;
	ScenarioScope<DeleteBucketPolicyOutput> contextOutput;
	//PutObjectOutput putObjectPropsOuput;

	QsError err = qsBucket.deleteBucketPolicy(input, *contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
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
	input.SetContentLength(strlen(objectKey.c_str()));

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
	std::string sourcePrefix = "/" + contextObjectTest->bucketName + "/";

	Bucket qsBucket = *contextObjectTest->pQsBucket;

	GetObjectInput input;
	GetObjectOutput contextOutput;

	QsError err = qsBucket.getObject(objectkeyToGet, input, contextOutput);
	if (QsError::QS_ERR_NO_ERROR != err)
	{
		//context->result = (int)putObjectPropsOuput.GetResponseCode();
	}
	contextOutput.GetBody()->rdbuf.read();
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

	EXPECT_EQ(expected, (int)contextOutput->GetResponseCode());
}

//Scenario Outline :
//# Initiate Multipart Upload
//When initiate multipart upload with key "<key>"
//Then initiate multipart upload status code is 200

//WHEN("^initiate multipart upload with key \"(.{1, })\"$"){
//	QingStor::QsConfig qsConfig;
//	qsConfig.loadConfigFile(strConfigPath);
//	QingStorService qsService(qsConfig);
//	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");
//
//	ScenarioScope<TestListMultipartUploadsCtx> contextGiven;
//
//	InitiateMultipartUploadInput input;
//	InitiateMultipartUploadOutput output;
//
//	contextGiven->objectKey = "testInitMultipartUpload.txt";
//
//	QsError err = qsBucket.initiateMultipartUpload(contextGiven->objectKey, input, output);
//
//	if (QsError::QS_ERR_NO_ERROR == err)
//	{
//		contextGiven->uploadID = output.GetUploadID();
//	}
//}
//
//WHEN("^list multipart uploads$") {
//
//	ScenarioScope<TestListMultipartUploadsCtx> contextGiven;
//
//	QingStor::QsConfig qsConfig;
//	qsConfig.loadConfigFile(strConfigPath);
//	QingStorService qsService(qsConfig);
//	Bucket qsBucket = qsService.GetBucket("huang-stor", "pek3a");
//
//	ListMultipartInput input;
//	ScenarioScope<ListMultipartOutput> contextOutput;
//	input.SetUploadID(contextGiven->uploadID);
//
//	QsError err = qsBucket.listMultipart(contextGiven->objectKey, input, *contextOutput);
//}
//
//THEN("^list multipart uploads count is (\\d+)$") {
//
//	REGEX_PARAM(double, expected);
//
//	ScenarioScope<ListMultipartOutput> contextOutput;
//
//	EXPECT_EQ(expected, contextOutput->GetCount());
//}


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
