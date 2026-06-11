class InterfaceClass
{
public:
    virtual ~InterfaceClass() = default;

    /**
     * @brief : A pure virtual method in InterfaceClass, making it an interface class
     */
    virtual void InterfaceMethod() = 0;

};

class TestClass
    : public InterfaceClass
{
public:
    /**
     * @brief : Constructor for TestClass
     */
    TestClass();

    ~TestClass() = default;

    /**
     * @brief : A static method in TestClass
     * @details : This method is used to demonstrate the parsing of static member functions in the PlantUML file.
     */
    static void StaticMethod();

    /**
     * @brief : A test method in TestClass
     * @details : This method is used to demonstrate the parsing of member functions in the PlantUML file.
     */
    void TestMethod();

    /**
     * @brief : Implementation of the pure virtual method from InterfaceClass
     */
    void InterfaceMethod() override;

private:
    // A private member variable in TestClass
    int test_variable;

};

class AbstractClass
{
public:
    virtual ~AbstractClass() = default;

    /**
     * @brief : A pure virtual method in AbstractClass, making it an abstract class
     */
    virtual void AbstractMethod() = 0;

};

class AnotherClass
    : public AbstractClass
{
public:
    /**
     * @brief : Constructor for AnotherClass
     */
    AnotherClass();

    ~AnotherClass() = default;

    /**
     * @brief : A test method in AnotherClass
     */
    void AnotherMethod();

    /**
     * @brief : Implementation of the pure virtual method from AbstractClass
     */
    void AbstractMethod() override;

private:
    // An instance of TestClass as a member variable in AnotherClass
    TestClass test_class_instance;

};

