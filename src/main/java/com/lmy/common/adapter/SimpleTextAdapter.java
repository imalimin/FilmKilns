package com.lmy.common.adapter;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.lmy.common.R;

/**
 * Created by lmy on 2017/4/12.
 */

public class SimpleTextAdapter extends RecyclerAdapter<String, RecyclerAdapter.BaseViewHolder<String>> {
    private int layoutRes;

    public SimpleTextAdapter() {
    }

    public SimpleTextAdapter(int layoutRes) {
        this.layoutRes = layoutRes;
    }

    @Override
    public void onBindViewHolder(BaseViewHolder<String> holder, String item, int position) {
        holder.onBind(item, position);
    }

    @Override
    public BaseViewHolder<String> onCreateViewHolder(ViewGroup parent, int viewType) {
        return new ViewHolder(LayoutInflater.from(parent.getContext()).inflate(layoutRes == 0 ? R.layout.item_simple_text : layoutRes, parent, false));
    }

    public static class ViewHolder extends BaseViewHolder<String> {
        private TextView textView;

        public ViewHolder(View itemView) {
            super(itemView);
            textView = (TextView) itemView.findViewById(android.R.id.text1);
        }

        @Override
        public void onBind(String item, int position) {
            textView.setText(item);
        }
    }
}
