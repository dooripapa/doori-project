// Copyright 2019 The doori-project Authors. All rights reserved.
// Use of this source code is governed by a GPL-3.0 license that can be found in the LICENSE file.
// See the AUTHORS file for names of contributors.
//
// Created by doori on 19. 7. 25.
//

#include "DataStructure/Tree.h"

namespace doori::DataStructure{

    template <typename T>
    auto Tree<T>::copyFrom(const Tree<T>& rhs) -> void
    {
        mRootBranches = std::move(rhs.mRootBranches);
    }

    template <typename T>
    auto Tree<T>::copyFrom(Tree<T>&& rhs) -> void
    {
        mRootBranches = std::move(rhs.mRootBranches);
    }

    template <typename T>
    Tree<T>::Tree() {

    }

    template <typename T>
    Tree<T>::Tree(const Tree<T>& rhs)
    {
        copyFrom(rhs);
    }

    template <typename T>
    Tree<T>::Tree(Tree<T>&& rhs)
    {
        copyFrom(std::move(rhs));
    }

    /**
     * Topic같은 branch를 찾기
     * @tparam T
     * @param topic_name_segment topic name 부분
     * @return tuple<bool, typename std::vector< doori_branch<T>>::iterator > 찾으면 첫인자는 true, 못찾으면 false, iterator는 대상 branch
     */
    template <typename T>
    auto Tree<T>::findRootBranches(const string& topic_name_segment, typename vector<Branch<T>>::iterator& itPos) -> bool
    {
        auto it=find_if(
                 mRootBranches.begin()
                ,mRootBranches.end()
                ,[=](const Branch<T>& branch){
                    return (branch.GetName() == topic_name_segment);
                }
        );
        auto ret=!(it==mRootBranches.end()); //찾으면 true, or false
        itPos=it;

        return ret;
    }
    template <typename T>
    auto Tree<T>::findRootBranches(const string& topic_name_segment) -> bool
    {
        auto it=find_if(
                 mRootBranches.begin()
                ,mRootBranches.end()
                ,[=](const Branch<T>& branch){
                    return (branch.GetName() == topic_name_segment);
                }
        );
        return !(it==mRootBranches.end()); //찾으면 true, or false
    }

    template <typename T>
    auto Tree<T>::attachBranch(const ILeafKey &dataKey, const Branch<T>& branch, INSERT mode) noexcept -> bool
    {
        auto ret=false;
        typename vector<Branch<T>>::iterator iter, linkIter;
        for(auto i=0; i < dataKey.GetDepth(); ++i)
        {
            if(i==0) {
                ret=findRootBranches(dataKey.GetDepthKey(i) , iter );
                if(!ret) {
                    Branch<T> aBranch(dataKey.GetDepthKey(i));
                    mRootBranches.push_back(aBranch);
                    findRootBranches(dataKey.GetDepthKey(i) , iter );
                }
            } else{
                ret = (*iter).FindLinkBranches(dataKey.GetDepthKey(i), linkIter);
                if(!ret) {
                    Branch<T> aBranch(dataKey.GetDepthKey(i));
                    (*iter).GetLinkBranches().push_back(aBranch);
                    (*iter).FindLinkBranches(dataKey.GetDepthKey(i), linkIter);
                }
                iter=linkIter;
            }
        }
        //해당 Branch에서 Linked Branches로부터 해당 중복된 branch가 있는지 확인함
        ret = (*iter).FindLinkBranches(branch.GetName(), linkIter);
        if( ret ) {
            if (mode==INSERT::CHECK)
                return false;
            else {
                (*iter).GetLinkBranches().erase(linkIter);
                (*iter).GetLinkBranches().push_back(branch);
            }
        }
        else
            (*iter).GetLinkBranches().push_back(branch);

        return true;
    }

    template <typename T>
    auto Tree<T>::removeBranch(const ILeafKey &dataKey) noexcept -> bool
    {
        int i;
        typename vector<Branch<T>>::iterator iter, linkIter;
        auto ret=false;
        auto depthMax = dataKey.GetDepth();
        for(i=0;i<depthMax;++i)
        {
            if(i==0)
                ret=findRootBranches(dataKey.GetDepthKey(i), iter );
            else
                ret= (*iter).FindLinkBranches(dataKey.GetDepthKey(i), linkIter);

            if(ret) {
                if(0<i && i<depthMax-1)
                    iter = linkIter;
                continue;
            }
            else
                break;
        }
        if(i == dataKey.GetDepth() && ret){
            (*iter).GetLinkBranches().erase(linkIter );
            LOG(INFO, "remove Branch TopicAccess:", dataKey.GetKeyName());
            return true;
        }
        return false;
    }

    template <typename T>
    auto Tree<T>::getBranch(const ILeafKey &dataKey) -> Branch<T>&
    {
        typename vector<Branch<T>>::iterator iter, linkIter;
        auto ret=false;
        for(auto i=0; i < dataKey.GetDepth(); ++i)
        {
            if(i==0) {
                ret=findRootBranches(dataKey.GetDepthKey(i), iter);
                //없으면 새로 branch 추가
                if(!ret) {
                    Branch<T> aBranch(dataKey.GetDepthKey(i));
                    mRootBranches.push_back(aBranch);
                    findRootBranches(dataKey.GetDepthKey(i), iter);
                    continue;
                }
            } else{
                ret= (*iter).FindLinkBranches(dataKey.GetDepthKey(i), linkIter);
                //없으면 새로 branch 추가
                if(!ret) {
                    Branch<T> aBranch(dataKey.GetDepthKey(i));
                    (*iter).GetLinkBranches().push_back(aBranch);
                    (*iter).FindLinkBranches(dataKey.GetDepthKey(i), linkIter);
                }
                iter=linkIter;
            }
        }
        return (*iter);
    }

    template <typename T>
    auto Tree<T>::attachLeaf(const ILeafKey &dataKey, T leaf) -> bool
    {
        //auto& ...
        //이렇게 해야, var 변수의 변경은, 값의 변경이 아닌, 참조의 값의 변환이 된다.
        auto& var = getBranch(dataKey);

        if(!var.IsThereLeaf(leaf))
            return false;

        var.AttachLeaf(leaf);
        return true;
    }

    template <typename T>
    auto Tree<T>::operator=(const Tree<T>& rhs) noexcept -> Tree<T>&
    {
        if( this ==&rhs  )
            return *this;

        copyFrom( rhs);
        return *this;
    }

    template <typename T>
    auto Tree<T>::operator=(Tree<T>&& rhs) noexcept -> Tree<T>&
    {
        if( this ==&rhs  )
            return *this;

        copyFrom( std::move(rhs) );
        return *this;
    }

    template<typename T>
    auto Tree<T>::RootBranches() noexcept -> vector<Branch<T>> & {
        return mRootBranches;
    }

}//namespace doori
