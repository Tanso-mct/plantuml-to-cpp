class TestClass
{
public:
    /**
     * @brief : This is a test method.
     */
    void TestMethod();

private:
    // This is a test variable.
    int test_variable;

};

class ChildClass
    : public TestClass
{
public:
    /**
     * @brief : This is a child method.
     */
    void ChildMethod();

    /**
     * @brief : This is a static child method.
     */
    static void StaticChildMethod();

protected:
    /**
     * @brief : This is a protected child method.
     */
    void ProtectedChildMethod();

private:
    // This is a child variable.
    int child_variable;

};

