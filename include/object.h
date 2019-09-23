#ifndef OBJECT_H
#define OBJECT_H

class object
{
    public:
        object();
        virtual ~object();
        int xTile,yTile;
        bool focus;
        virtual void openTile(Game* game) =0;
    private:
};

#endif // OBJECT_H
