template <typename T>
class Service
{
public:
    Service(T &_inst)
        : instance(inst)
    {

    }

    ~Service()
    {
        
    }

    T &get() {
        return instance;
    }

private:
    T &instance;
};