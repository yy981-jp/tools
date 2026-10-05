# y9inc

STLを補助するためのC++ユーティリティを提供するライブラリです。

必要な機能を個別に利用できるよう、submoduleとしての利用も想定しています。

## Documentation
> [!NOTE]
詳細なAPI仕様については、[Documentation](https://yy981-jp.github.io/tools/files.html)をご確認ください。


## string.h
> [!TIP]
`string_view`を引数には積極的に使いますが、返り値には原則使用しません。

ある程度メモリコピーのコストは抑えたうえで、複雑な寿命管理をしないで済むように設計されています。
