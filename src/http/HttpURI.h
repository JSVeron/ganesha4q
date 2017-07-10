#ifndef QS_HTTP_URI_H
#define QS_HTTP_URI_H

#include  "QsSdkConfig.h"
#include "http/HttpCommon.h"

#include <stdint.h>
#include <string>
#include <map>


namespace QingStor
{
    namespace Http
    {

        extern QS_SDK_API const char* SEPARATOR;
        static const uint16_t HTTP_DEFAULT_PORT = 80;
        static const uint16_t HTTPS_DEFAULT_PORT = 443;

        //per https://tools.ietf.org/html/rfc3986#section-3.4 there is nothing preventing servers from allowing
        //multiple values for the same key. So use a multimap instead of a map.
        typedef std::multimap<std::string, std::string> QueryStringParameterCollection;

        /**
         * class modeling universal resource identifier, but implemented for http
         */
        class QS_SDK_API URI
        {
        public:
            /**
              * Defaults to http and port 80
              */
            URI();
            /**
              * Parses string and sets uri fields
              */
            URI(const std::string&);
            /**
              * Parses string and sets uri fields
              */
            URI(const char*);

            URI& operator = (const std::string&);
            URI& operator = (const char*);

            bool operator == (const URI&) const;
            bool operator == (const std::string&) const;
            bool operator == (const char*) const;
            bool operator != (const URI&) const;
            bool operator != (const std::string&) const;
            bool operator != (const char*) const;

            /**
            * scheme or protocol e.g. http, https, ftp
            */
            inline Scheme GetScheme() const { return m_scheme; }

            /**
            * Sets scheme, if the port is incompaitible with this scheme, the port will automatically be set as well.
            */
            void SetScheme(Scheme value);

            /**
            * Gets the domain portion of the uri
            */
            inline const std::string& GetAuthority() const { return m_authority; }

            /**
            * Sets the domain portion of the uri
            */
            inline void SetAuthority(const std::string& value) { m_authority = value; }

            /**
            * Gets the port portion of the uri, defaults to 22 for ftp, 80 for http and 443 for https
            */
            inline uint16_t GetPort() const { return m_port; }

            /**
            * Sets the port portion of the uri, normally you will not have to do this. If the scheme is set to ftp, http
            * or https then the default ports will be set.
            */
            inline void SetPort(uint16_t value) { m_port = value; }

            /**
            * Gets the path portion of the uri e.g. the portion after the first slash after the authority and prior to the
            * query string. This is not url encoded.
            */
            inline const std::string& GetPath() const { return m_path; }

            /**
            * Gets the path portion of the uri, url encodes it and returns it
            */
            inline std::string GetURLEncodedPath() const { return URLEncodePath(m_path); }

            /**
            * Sets the path portion of the uri. URL encodes it if needed
            */
            void SetPath(const std::string& value);

            /**
            * Gets the raw query string including the ?
            */
            inline const std::string& GetQueryString() const { return m_queryString; }

            /**
             * Resets the query string to the raw string. all query string manipulations made before this call will be lost
             */
            void SetQueryString(const std::string& str);

            std::string GetFormParameters() const;

            /**
            * Cannonicalizes the query string.
            */
            void CanonicalizeQueryString();

            /**
            * parses query string and returns url decoded key/value mappings from it. Spaces and all url encoded
            * values will not be encoded.
            */
            QueryStringParameterCollection GetQueryStringParameters(bool decode = true) const;

            /**
            * Adds query string parameter to underlying query string.
            */
            void AddQueryStringParameter(const char* key, const std::string& value);

            /**
            * Converts the URI to a String usable for its context. e.g. an http request.
            */
            std::string GetURIString(bool includeQueryString = true) const;

            /**
             * URLEncodes the path portions of path (doesn't encode the "/" portion)
             */
            static std::string URLEncodePath(const std::string& path);

        private:
            void ParseURIParts(const std::string& uri);
            void ExtractAndSetScheme(const std::string& uri);
            void ExtractAndSetAuthority(const std::string& uri);
            void ExtractAndSetPort(const std::string& uri);
            void ExtractAndSetPath(const std::string& uri);
            void ExtractAndSetQueryString(const std::string& uri);
            bool CompareURIParts(const URI& other) const;

            Scheme m_scheme;
			std::string m_authority;
            uint16_t m_port;
            std::string m_path;
            std::string m_queryString;
        };

    } // namespace Http
} // namespace Aws

#endif // QS_HTTP_URI_H
