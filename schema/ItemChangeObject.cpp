// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

// WARNING! Do not edit this file manually, your changes will be overwritten.

#include "ItemChangeObject.h"

#include "graphqlservice/internal/Schema.h"

#include "graphqlservice/introspection/IntrospectionSchema.h"

using namespace std::literals;

namespace graphql::mapi {
namespace object {

ItemChange::ItemChange(std::unique_ptr<Concept>&& pimpl) noexcept
	: service::Object { pimpl->getTypeNames(), pimpl->getResolvers() }
	, _pimpl { std::move(pimpl) }
{
}

void ItemChange::beginSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->beginSelectionSet(params);
}

void ItemChange::endSelectionSet(const service::SelectionSetParams& params) const
{
	_pimpl->endSelectionSet(params);
}

} // namespace object

void AddItemChangeDetails(const std::shared_ptr<schema::UnionType>& typeItemChange, const std::shared_ptr<schema::Schema>& schema)
{
	typeItemChange->AddPossibleTypes({
		schema->LookupType(R"gql(ItemAdded)gql"sv),
		schema->LookupType(R"gql(ItemUpdated)gql"sv),
		schema->LookupType(R"gql(ItemRemoved)gql"sv),
		schema->LookupType(R"gql(ItemsReloaded)gql"sv)
	});
}

} // namespace graphql::mapi
