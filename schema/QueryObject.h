// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#pragma once

#ifndef QUERYOBJECT_H
#define QUERYOBJECT_H

#include "MAPISchema.h"

namespace graphql::mapi::object {

class Query
	: public service::Object
{
protected:
	explicit Query();

public:
	virtual service::FieldResult<std::vector<std::shared_ptr<Store>>> getStores(service::FieldParams&& params, std::optional<std::vector<response::IdType>>&& idsArg) const = 0;

private:
	std::future<service::ResolverResult> resolveStores(service::ResolverParams&& params);

	std::future<service::ResolverResult> resolve_typename(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolve_schema(service::ResolverParams&& params);
	std::future<service::ResolverResult> resolve_type(service::ResolverParams&& params);

	std::shared_ptr<schema::Schema> _schema;
};

} // namespace graphql::mapi::object

#endif // QUERYOBJECT_H
