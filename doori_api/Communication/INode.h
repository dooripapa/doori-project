//
// Created by KOSCOM on 2024-01-10.
//

#ifndef DOORI_PROJECT_INODE_H
#define DOORI_PROJECT_INODE_H

namespace doori::api::Communication{
    template <typename T, typename TStrategy>
    class INode
    {
        using FromStrategy = std::function<int(T)>;
        using ToStrategy = std::function<int(T)>;
        using HandleStrategy = std::function<int(T)>;
    public:
        int From(){

        };
        int To(){

        };
        int H(){

        };


    private:



    };
}//doori

#endif //DOORI_PROJECT_INODE_H
