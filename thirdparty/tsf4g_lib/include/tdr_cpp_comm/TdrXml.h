/* This file is generated by tdr. */
/* No manual modification is permitted. */

/* tdr version: 2.7.12 */

#ifndef TSF4G_TDR_TDR_XML_H_
#define TSF4G_TDR_TDR_XML_H_

#include "TdrPal.h"
#include "TdrError.h"
#include "TdrIO.h"
#include "TdrBuf.h"
#include "rapidxml/rapidxml.hpp"

namespace tsf4g_tdr
{


class TdrXmlReader
{
    private:
        char* _buffer;
        TdrXmlFormat _format;

    private:
        TdrError::ErrorType _state;

    private:
        rapidxml::xml_document<>  _ducument;
        rapidxml::xml_node<>     *_currRoot;
        rapidxml::xml_node<>     *_currNode;

    private:
        TdrError::ErrorType _parse(char* string);

    public:
        TdrXmlReader(char* string, TdrXmlFormat format);

        TdrXmlReader(const char* buffer, size_t length, TdrXmlFormat format);

        ~TdrXmlReader()
        {
            if (NULL != _buffer)
            {
                delete [] _buffer;
                _buffer = NULL;
            }
        }

    public:
        TdrError::ErrorType state() const
        {
            return _state;
        }

    public:
        enum WalkState {
            WS_NORMAL,
            WS_NO_MORE_NODE,
        };

    public:
        WalkState stepIn(const char* name);
        void stepOut(const char* name);

    public:
        const char* getEntryValue(const char* name);

    public:
        const char* getNodeValue(const char* name);
        const char* getAttrValue(const char* name);
};


class TdrXmlWriter
{
    private:
        TdrOutStream _out;
        TdrXmlFormat _format;

    private:
        TdrError::ErrorType _state;

    private:
        enum {
            TDR_MAX_INDENT_LEVEL = 32,
        };

    private:
        bool        _isSimple;
        bool        _isPartial;
        unsigned    _indentLvl;
        char        _indentStr[TDR_MAX_INDENT_LEVEL + 1];

    private:
        void _init()
        {
            _isSimple = false;
            _isPartial = false;
            _indentLvl = 0;
            _indentStr[0] = 0;
            _indentStr[TDR_MAX_INDENT_LEVEL] = 0;
        }

    private:
        void _indent()
        {
            if (_indentLvl < TDR_MAX_INDENT_LEVEL)
            {
                _indentStr[_indentLvl] = '\t';
            }
            ++_indentLvl;
            if (_indentLvl < TDR_MAX_INDENT_LEVEL)
            {
                _indentStr[_indentLvl] = 0;
            }
        }

        void _dedent()
        {
            if (_indentLvl < TDR_MAX_INDENT_LEVEL)
            {
                _indentStr[_indentLvl] = '\t';
            }
            --_indentLvl;
            if (_indentLvl < TDR_MAX_INDENT_LEVEL)
            {
                _indentStr[_indentLvl] = 0;
            }
        }

    public:
        TdrXmlWriter(FILE* fp, TdrXmlFormat format);
        TdrXmlWriter(char* ptr, size_t size, TdrXmlFormat format);

    public:
        TdrError::ErrorType state() const
        {
            return _state;
        }

    public:
        size_t getUsedSize() const
        {
            return _out.getUsedSize();
        }

    public:
        TdrError::ErrorType openSimple(const char* name);
        TdrError::ErrorType closeSimple(const char* name);

        TdrError::ErrorType openComplex(const char* name);
        TdrError::ErrorType closeComplex(const char* name);

    public:
        TdrError::ErrorType openAttr(const char* name)
        {
            assert(TdrError::TDR_NO_ERROR == _state);

            _state = _out.textize(" %s=\"", name);

            return _state;
        }

        TdrError::ErrorType closeAttr()
        {
            assert(TdrError::TDR_NO_ERROR == _state);

            _state = _out.textize("\"");

            return _state;
        }

        TdrError::ErrorType openNode(const char* name)
        {
            assert(TdrError::TDR_NO_ERROR == _state);

            if (_isPartial)
            {
                _state = _out.textize(">\n%s<%s> ", _indentStr, name);
                _isPartial = false;
            } else
            {
                _state = _out.textize("\n%s<%s> ", _indentStr, name);
            }

            return _state;
        }

        TdrError::ErrorType closeNode(const char* name)
        {
            assert(TdrError::TDR_NO_ERROR == _state);

            assert(false == _isPartial);

            _state = _out.textize("</%s>\n", name);

            return _state;
        }

    public:
        TdrError::ErrorType textize(const char* format, ...)
        {
            assert(TdrError::TDR_NO_ERROR == _state);
            assert(NULL != format);

            va_list ap;

            va_start(ap, format);
            _state = _out.vtextize(format, ap);
            va_end(ap);

            return _state;
        }

        TdrError::ErrorType description(const char* desc)
        {
            assert(TdrError::TDR_NO_ERROR == _state);
            assert(NULL != desc);

            if (_isPartial)
            {
                _state = _out.textize(">\n%s<!-- ",  _indentStr);
                _isPartial = false;
            }
            else
            {
                _state = _out.textize("\n%s<!-- ",  _indentStr);
            }

            if (_state != TdrError::TDR_NO_ERROR)
            {
                return _state;
            }

            _state = _out.textize(desc);
            if (_state != TdrError::TDR_NO_ERROR)
            {
                return _state;
            }

            _state = _out.textize(" -->");

            return _state;
        }

    public:
        TdrError::ErrorType declare(const char* encoding)
        {
            return _out.textize("<?xml version=\"1.0\" encoding=\"%s\" standalone=\"yes\" ?>\n", encoding);
        }

    public:
        TdrError::ErrorType textizeStr(const char* src, bool space);

    public:
        TdrError::ErrorType textizeIP(tdr_ip_t src, bool space);
        TdrError::ErrorType textizeDate(tdr_date_t src, bool space);
        TdrError::ErrorType textizeTime(tdr_time_t src, bool space);
        TdrError::ErrorType textizeDateTime(tdr_datetime_t src, bool space);
};

}
#endif
