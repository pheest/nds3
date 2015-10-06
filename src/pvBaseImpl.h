#ifndef NDSPVBASEIMPL_H
#define NDSPVBASEIMPL_H

#include "baseImpl.h"
#include "../include/nds3/definitions.h"

#include <string>

namespace nds
{

class PVBase;

/**
 * @brief Base class for all the PVs.
 */
class PVBaseImpl: public BaseImpl
{
public:
    PVBaseImpl(const std::string& name);

    // All the base versions throw.
    // Only the overwritten ones in the derived classes will function correctly.
    ////////////////////////////////////////////////////////////////////////////
    virtual void read(timespec* pTimestamp, std::int32_t* pValue);
    virtual void read(timespec* pTimestamp, double* pValue);
    virtual void read(timespec* pTimestamp, std::vector<std::int8_t>* pValue);
    virtual void read(timespec* pTimestamp, std::vector<std::uint8_t>* pValue);
    virtual void read(timespec* pTimestamp, std::vector<std::int32_t>* pValue);
    virtual void read(timespec* pTimestamp, std::vector<double>* pValue);

    virtual void write(const timespec& timestamp, const std::int32_t& value);
    virtual void write(const timespec& timestamp, const double& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::uint8_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<std::int32_t>& value);
    virtual void write(const timespec& timestamp, const std::vector<double>& value);

    template<typename T>
    void push(const timespec& timestamp, const T& value);

    virtual void initialize(FactoryBaseImpl& controlSystem);

    virtual void deinitialize();

    /**
     * @brief Return the PV's data type.
     *
     * Used to declare Tango attributes or when auto-generating db files for EPICS.
     *
     * @return the data type used by the PV
     */
    virtual dataType_t getDataType() = 0;

    void setType(const recordType_t type);
    void setDescription(const std::string& description);
    void setInterfaceName(const std::string& interfaceName);
    void setScanType(const scanType_t scanType, const double periodSeconds);
    void setMaxElements(const size_t maxElements);
    void setEnumeration(const enumerationStrings_t& enumerations);

    recordType_t getType() const;
    std::string getDescription() const;
    std::string getInterfaceName() const;
    scanType_t getScanType() const;
    double getScanPeriodSeconds() const;
    size_t getMaxElements() const;
    const enumerationStrings_t& getEnumerations();


protected:
    template<typename T>
#if !defined(__GNUC__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)
    constexpr
#endif
    dataType_t getDataTypeForCPPType() const
    {
        const int type =
                int(std::is_same<T, std::int32_t>::value) * (int)dataType_t::dataInt32 +
                int(std::is_same<T, double>::value) * (int)dataType_t::dataFloat64 +
                int(std::is_same<T, std::vector<std::int8_t> >::value) * (int)dataType_t::dataInt8Array +
                int(std::is_same<T, std::vector<std::uint8_t> >::value) * (int)dataType_t::dataUint8Array +
                int(std::is_same<T, std::vector<std::int32_t> >::value) * (int)dataType_t::dataInt32Array +
                int(std::is_same<T, std::vector<double> >::value) * (int)dataType_t::dataFloat64Array +
                int(std::is_same<T, std::string>::value) * (int)dataType_t::dataString;

        static_assert(type != 0, "Undefined data type");
        return(dataType_t)type;
    }

    recordType_t m_type;
    std::string m_description;
    std::string m_interfaceName;
    scanType_t m_scanType;
    double m_periodicScanSeconds;
    size_t m_maxElements;
    enumerationStrings_t m_enumeration;
};

}
#endif // NDSPVBASEIMPL_H