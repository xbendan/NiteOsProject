#pragma once

#include <common/cstring.h>
#include <utils/optional.h>

enum class ResultResponse
{
    /// @brief Success, objPtr should now contains a correct value
    Success                  = 0,
    /// @brief Failed with unknown error (undefined)
    Failed                   = 1,
    /// @brief Try to access array with invalid index
    OutOfBoundException      = (1 << 1),
    /// @brief Passed a null pointer while it cannot be accepted
    NullPointerException     = (1 << 2),
    /// @brief Argument check failed, not what function wants
    IllegalArgumentException = (1 << 3),
    /// @brief Action stopped by security check
    SecurityException        = (1 << 4),
    /// @brief String contained unrecognizable format
    StringFormatException    = (1 << 5),
    /// @brief Problem within io operation
    IOException              = (1 << 6),
    /// @brief Generic type does not match requirements
    TypeValidateException    = (1 << 7)
};

using namespace utils;

/**
 * @brief This class usually represent a result of function or operation
 * Result is a derived class of Optional, represents the result of an operation.
 * It should contains a correct pointer to return type, and can retrieve by
 * @code get() @endcode , unless the return type was set to @code Void @endcode
 * . Meanwhile, you should always check whether the operation is success by
 * @code isSuccess() @endcode , if not, the error type can be get by calling
 * @code getResponse() @endcode , some extra information will be provided by
 * @code getMessage() @endcode .
 *
 * @see utils::Optional
 * @tparam T Return Type
 */
template <typename T>
class Result : public utils::Optional<T>
{
public:
    Result() = default;
    Result(ResultResponse response)
      : Optional<T>(nullptr)
      , m_response(response)
    {
    }
    Result(T* const objPtr)
      : Optional<T>(objPtr)
      , m_response(ResultResponse::Success)
    {
    }
    Result(T* const objPtr, ResultResponse response)
      : Optional<T>(objPtr)
      , m_response(response)
    {
    }
    ~Result() {}

    bool           isSuccess() { return m_response == ResultResponse::Success; }
    ResultResponse getResponse() { return m_response; }
    String&        getMessage() { return m_message; }

private:
    String         m_message;
    ResultResponse m_response;
};