/*
   Copyright 2010-2018 Amazon.com, Inc. or its affiliates. All Rights Reserved.
   This file is licensed under the Apache License, Version 2.0 (the "License").
   You may not use this file except in compliance with the License. A copy of
   the License is located at
    http://aws.amazon.com/apache2.0/
   This file is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied. See the License for the
   specific language governing permissions and limitations under the License.
*/

#include <aws/core/Aws.h>
#include <aws/sns/SNSClient.h>
#include <aws/sns/model/PublishRequest.h>
#include <aws/sns/model/PublishResult.h>
#include <aws/core/utils/logging/DefaultLogSystem.h>
#include <aws/core/utils/logging/AWSLogging.h>
#include <iostream>

/**
 * Publish sms
 */

int main(int argc, char ** argv)
{
    if (argc != 4)
    {
        std::cout << "Usage: publish_sms <sender_id> <message_value> <topic_arn_value> " << std::endl;
        return 1;
    }

    Aws::SDKOptions options;
    Aws::InitAPI(options);
    Aws::Utils::Logging::InitializeAWSLogging(
        Aws::MakeShared<Aws::Utils::Logging::DefaultLogSystem>(
            "sns_test", Aws::Utils::Logging::LogLevel::Trace, "aws_sdk_"));

    {
        Aws::Client::ClientConfiguration client_configuration;
        client_configuration.region = "eu-west-1";
        Aws::SNS::SNSClient sns(client_configuration);

        Aws::String sender_id = argv[1];
        Aws::String message = argv[2];
        Aws::String topic_arn = argv[3];

        // Define message attributes
        // SenderID
        Aws::SNS::Model::MessageAttributeValue attr_sender_id;
        attr_sender_id.SetDataType("String");
        attr_sender_id.SetStringValue(sender_id);
        Aws::String attr_name_sender_id = "AWS.SNS.SMS.SenderID";
        // Transaction SMS
        Aws::SNS::Model::MessageAttributeValue attr_txsms_type;
        attr_txsms_type.SetDataType("String");
        attr_txsms_type.SetStringValue("Transactional");
        Aws::String attr_name_txsms_type = "AWS.SNS.SMS.SMSType";

        Aws::SNS::Model::PublishRequest psms_req;
        psms_req.SetMessage(message);
        psms_req.SetTopicArn(topic_arn);
        psms_req.AddMessageAttributes(attr_name_txsms_type, attr_txsms_type);   // Add TXSMS attribute
        psms_req.AddMessageAttributes(attr_name_sender_id, attr_sender_id);     // Add SenderID attribute


        // Print environment status
        std::cout << "Region: " << client_configuration.region << std::endl;
        std::cout << "Topic: " << psms_req.GetTopicArn() << std::endl;
        //Aws::Map<Aws::String, Aws::SNS::Model::MessageAttributeValue> message_attrs = psms_req.GetMessageAttributes();
        std::cout << "Attributes:" << std::endl;
        for(auto elem : psms_req.GetMessageAttributes())
        {
            std::cout << "    " << elem.first << ": { DataType: " 
               << elem.second.GetDataType() << ", StringValue: " 
               << elem.second.GetStringValue() << " }" << std::endl;
        }
 
        std::cout << std::endl;
        auto psms_out = sns.Publish(psms_req);

        if (psms_out.IsSuccess())
        {
        std::cout << "Message published successfully " << std::endl;
        }
        else
        {
        std::cout << "Error while publishing message " << psms_out.GetError().GetMessage()
            << std::endl;
        }

    }

    Aws::Utils::Logging::ShutdownAWSLogging();
    Aws::ShutdownAPI(options);
    return 0;
}