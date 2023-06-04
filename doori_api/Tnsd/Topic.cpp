// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//
#include "Topic.h"

using namespace std;

namespace doori::Tnsd{

    Topic::Topic() : mTopics{} {
    }

    Topic::Topic(const Topic &topic) {
        copyFrom(topic);
    }

    Topic::Topic(Topic &&topic) {
        copyFrom(move(topic));
    }

    Topic::Topic(initializer_list<string> topicList) {
        mTopics.clear();
        mTopics.insert(mTopics.end(), topicList.begin(),topicList.end());
    }

    auto Topic::set(initializer_list<string> topicList) -> bool
    {
        mTopics.clear();
        for(auto it=topicList.begin();it!=topicList.end();++it)
        {
            size_t found = (*it).find_first_of(" .\n\t");
            if( found != std::string::npos )
            {
                LOG(ERROR, "not allow Character");
                mTopics.clear();
                mTopicSum="";
                return false;
            }
            mTopics.push_back(*it);
            if (it+1 != topicList.end())
                mTopicSum+=(*it)+".";
            else
                mTopicSum+=(*it);
        }
        return true;
    }

    auto Topic::set(const string& topic_pause_sets ) -> bool
    {
        mTopics.clear();
        size_t found;
        mTopicSum = topic_pause_sets;
        std::size_t startPos=0, len=0;

        //check
        found = topic_pause_sets.find_first_of(" \n\t");
        if( found != std::string::npos )
        {
            LOG(ERROR, "not allow Character");
            mTopicSum="";
            return false;
        }

        do {
            found=topic_pause_sets.find_first_of(".", startPos);
            if(found==std::string::npos)
                len = mTopicSum.size() - startPos;
            else
                len = found - startPos;

            mTopics.push_back(mTopicSum.substr(startPos,len));

            startPos = found+1;
        } while (found<(mTopicSum.size()-1));

        return true;
    }

    auto Topic::operator==(const Topic& rhs) const noexcept -> bool
    {
        return (mTopicSum == rhs.mTopicSum);
    }


    auto Topic::getTopicName() const noexcept -> std::string
    {
        return mTopicSum;
    }

    auto Topic::getTopicName(const unsigned int index) const noexcept -> std::string {
        if(mTopics.size()<=index)
        {
            LOG(FATAL, "Index is overflow");
            return "";
        }
        else
            return mTopics[index];
    }

    auto Topic::getDepthSize() const noexcept -> uint
    {
        return mTopics.size();
    }

    auto Topic::operator=(Topic &&rhs) noexcept -> Topic&{
        if ( this==&rhs )
            return *this;

        mTopics=std::move(rhs.mTopics);
        mTopicSum=std::move(rhs.mTopicSum);
        return *this;
    }

    auto Topic::operator=(const Topic &rhs) noexcept -> Topic& {
        if ( this==&rhs )
            return *this;

        copyFrom(rhs);
        return *this;
    }

    auto Topic::copyFrom(const Topic &rhs) noexcept -> void {
        mTopics=rhs.mTopics;
        mTopicSum=rhs.mTopicSum;
    }


}//namespace doori
