/*
 * Copyright (c) 2022, Andrew Kaster <akaster@serenityos.org>
 * Copyright (c) 2023, Luke Wilde <lukew@serenityos.org>
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#pragma once

#include <AK/Vector.h>
#include <LibJS/Heap/GCPtr.h>
#include <LibWeb/Bindings/PlatformObject.h>
#include <LibWeb/FileAPI/File.h>
#include <LibWeb/WebIDL/Types.h>

namespace Web::FileAPI {

class FileList
    : public Bindings::PlatformObject
    , public Bindings::Serializable {
    WEB_PLATFORM_OBJECT(FileList, Bindings::PlatformObject);
    JS_DECLARE_ALLOCATOR(FileList);

public:
    [[nodiscard]] static JS::NonnullGCPtr<FileList> create(JS::Realm&, Vector<JS::NonnullGCPtr<File>>&&);
    [[nodiscard]] static JS::NonnullGCPtr<FileList> create(JS::Realm&);

    virtual ~FileList() override;

    // https://w3c.github.io/FileAPI/#dfn-length
    WebIDL::UnsignedLong length() const { return m_files.size(); }

    // https://w3c.github.io/FileAPI/#dfn-item
    File* item(size_t index)
    {
        return index < m_files.size() ? m_files[index].ptr() : nullptr;
    }

    // https://w3c.github.io/FileAPI/#dfn-item
    File const* item(size_t index) const
    {
        return index < m_files.size() ? m_files[index].ptr() : nullptr;
    }

    virtual bool is_supported_property_index(u32 index) const override;
    virtual WebIDL::ExceptionOr<JS::Value> item_value(size_t index) const override;

    virtual StringView interface_name() const override { return "FileList"sv; }
    virtual WebIDL::ExceptionOr<void> serialization_steps(HTML::SerializationRecord& serialized, bool for_storage, HTML::SerializationMemory&) override;
    virtual WebIDL::ExceptionOr<void> deserialization_steps(ReadonlySpan<u32> const& serialized, size_t& position, HTML::DeserializationMemory&) override;

private:
    FileList(JS::Realm&, Vector<JS::NonnullGCPtr<File>>&&);
    explicit FileList(JS::Realm&);

    virtual void initialize(JS::Realm&) override;
    virtual void visit_edges(Cell::Visitor&) override;

    Vector<JS::NonnullGCPtr<File>> m_files;
};

}
