package com.lmy.common.widget;

import android.animation.Animator;
import android.animation.AnimatorListenerAdapter;
import android.animation.TimeInterpolator;
import android.animation.ValueAnimator;
import android.os.Build;
import android.os.Handler;
import android.view.View;

/**
 * Created by 李明艺 on 2017/1/17.
 *
 * @author lrlmy@foxmail.com
 */

public class ViewCompat {
    public static ViewValueAnimator animate(View view) {
        return new ViewValueAnimator(view);
    }

    public static class ViewValueAnimator implements ValueAnimator.AnimatorUpdateListener {
        static final float NO_VALUE = Float.MIN_VALUE;
        /**
         * Constants used to associate a property being requested and the mechanism used to set
         * the property (this class calls directly into View to set the properties in question).
         */
        static final int TRANSLATION_X = 0;
        static final int TRANSLATION_Y = 1;
        static final int TRANSLATION_Z = 2;
        static final int SCALE_X = 3;
        static final int SCALE_Y = 4;
        static final int ROTATION = 5;
        static final int ROTATION_X = 6;
        static final int ROTATION_Y = 7;
        static final int X = 8;
        static final int Y = 9;
        static final int Z = 10;
        static final int ALPHA = 11;
        static final int WIDTH = 12;
        static final int HEIGHT = 13;
        static final int SCROLL_X = 14;
        static final int SCROLL_Y = 15;

        private float[] deltaValue;
        private float[] fromValue;

        private Handler mHandler;
        private long mStartDelayed, mEndDelayed;
        private Runnable mStartAction, mEndAction;
        private View mView;
        private ValueAnimator animator;
        private AnimatorListenerAdapter animatorListenerAdapter = new AnimatorListenerAdapter() {
            @Override
            public void onAnimationEnd(Animator animation) {
                super.onAnimationEnd(animation);
                if (mEndAction != null)
                    mHandler.postDelayed(mEndAction, mEndDelayed);
            }

            @Override
            public void onAnimationStart(Animator animation) {
                super.onAnimationStart(animation);
                if (mStartAction != null)
                    mHandler.postDelayed(mStartAction, mStartDelayed);
            }
        };

        /*Listener START------------*/
        @Override
        public void onAnimationUpdate(ValueAnimator animation) {
            animation((float) animation.getAnimatedValue());
        }

        protected int getCount() {
            return SCROLL_Y + 1;
        }

        public ViewValueAnimator(View view) {
            this.mView = view;
            this.mHandler = new Handler();
            animator = ValueAnimator.ofFloat(0f, 1f);
            animator.addUpdateListener(this);
            animator.addListener(animatorListenerAdapter);
        }

        protected void setDeltaValue(int propertyConstant, float value) {
            if (this.deltaValue == null)
                this.deltaValue = new float[getCount()];
            this.deltaValue[propertyConstant] = value;
        }

        protected float getDeltaValue(int propertyConstant) {
            return this.deltaValue[propertyConstant];
        }

        protected void setFromValue(int propertyConstant, float value) {
            if (this.fromValue == null)
                this.fromValue = new float[getCount()];
            this.fromValue[propertyConstant] = value;
        }

        protected float getFromValue(int propertyConstant) {
            return this.fromValue[propertyConstant];
        }

        protected boolean hasAnimation(int propertyConstant) {
            return null != this.deltaValue && this.deltaValue[propertyConstant] != 0;
        }

        public ViewValueAnimator setDuration(long duration) {
            animator.setDuration(duration);
            return this;
        }

        public long getDuration() {
            return animator.getDuration();
        }

        public long getStartDelay() {
            return animator.getStartDelay();
        }

        public ViewValueAnimator setStartDelay(long startDelay) {
            animator.setStartDelay(startDelay);
            return this;
        }

        public ViewValueAnimator setInterpolator(TimeInterpolator interpolator) {
            animator.setInterpolator(interpolator);
            return this;
        }

        public TimeInterpolator getInterpolator() {
            return animator.getInterpolator();
        }

        public ViewValueAnimator addListener(Animator.AnimatorListener listener) {
            animator.addListener(listener);
            return this;
        }

        public ViewValueAnimator addUpdateListener(ValueAnimator.AnimatorUpdateListener listener) {
            animator.addUpdateListener(listener);
            return this;
        }

        public ViewValueAnimator addPauseListener(ValueAnimator.AnimatorPauseListener listener) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
                animator.addPauseListener(listener);
            }
            return this;
        }

        public void start() {
            animator.start();
        }

        public void cancel() {
            animator.cancel();
        }

        public boolean isRunning() {
            return animator.isRunning();
        }

        public boolean isStarted() {
            return animator.isStarted();
        }

        public ViewValueAnimator width(float value) {
            animateProperty(WIDTH, value);
            return this;
        }

        public ViewValueAnimator height(float value) {
            animateProperty(HEIGHT, value);
            return this;
        }

        public ViewValueAnimator x(float value) {
            animateProperty(X, value);
            return this;
        }

        public ViewValueAnimator xBy(float value) {
            animatePropertyBy(X, value);
            return this;
        }

        public ViewValueAnimator y(float value) {
            animateProperty(Y, value);
            return this;
        }

        public ViewValueAnimator yBy(float value) {
            animatePropertyBy(Y, value);
            return this;
        }

        public ViewValueAnimator z(float value) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                animateProperty(Z, value);
            }
            return this;
        }

        public ViewValueAnimator zBy(float value) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                animatePropertyBy(Z, value);
            }
            return this;
        }

        public ViewValueAnimator rotation(float value) {
            animateProperty(ROTATION, value);
            return this;
        }

        public ViewValueAnimator rotationBy(float value) {
            animatePropertyBy(ROTATION, value);
            return this;
        }

        public ViewValueAnimator rotationX(float value) {
            animateProperty(ROTATION_X, value);
            return this;
        }

        public ViewValueAnimator rotationXBy(float value) {
            animatePropertyBy(ROTATION_X, value);
            return this;
        }

        public ViewValueAnimator rotationY(float value) {
            animateProperty(ROTATION_Y, value);
            return this;
        }

        public ViewValueAnimator rotationYBy(float value) {
            animatePropertyBy(ROTATION_Y, value);
            return this;
        }

        public ViewValueAnimator translationX(float value) {
            animateProperty(TRANSLATION_X, value);
            return this;
        }

        public ViewValueAnimator translationXBy(float value) {
            animatePropertyBy(TRANSLATION_X, value);
            return this;
        }

        public ViewValueAnimator translationY(float value) {
            animateProperty(TRANSLATION_Y, value);
            return this;
        }

        public ViewValueAnimator translationYBy(float value) {
            animatePropertyBy(TRANSLATION_Y, value);
            return this;
        }

        public ViewValueAnimator translationZ(float value) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                animateProperty(TRANSLATION_Z, value);
                return this;
            }
            return this;
        }

        public ViewValueAnimator translationZBy(float value) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                animatePropertyBy(TRANSLATION_Z, value);
            }
            return this;
        }

        public ViewValueAnimator scaleX(float value) {
            animateProperty(SCALE_X, value);
            return this;
        }

        public ViewValueAnimator scaleXBy(float value) {
            animatePropertyBy(SCALE_X, value);
            return this;
        }

        public ViewValueAnimator scaleY(float value) {
            animateProperty(SCALE_Y, value);
            return this;
        }

        public ViewValueAnimator scaleYBy(float value) {
            animatePropertyBy(SCALE_Y, value);
            return this;
        }

        public ViewValueAnimator alpha(float value) {
            animateProperty(ALPHA, value);
            return this;
        }

        public ViewValueAnimator alphaBy(float value) {
            animatePropertyBy(ALPHA, value);
            return this;
        }

        public ViewValueAnimator scrollX(float value) {
            animateProperty(SCROLL_X, value);
            return this;
        }

        public ViewValueAnimator scrollY(float value) {
            animateProperty(SCROLL_Y, value);
            return this;
        }

        public ViewValueAnimator withStartAction(Runnable runnable) {
            return withStartDelayedAction(runnable, 0);
        }

        public ViewValueAnimator withStartDelayedAction(Runnable runnable, long delayed) {
            this.mStartDelayed = delayed;
            this.mStartAction = runnable;
            return this;
        }

        public ViewValueAnimator withEndAction(Runnable runnable) {
            return withEndDelayedAction(runnable, 0);
        }

        public ViewValueAnimator withEndDelayedAction(Runnable runnable, long delayed) {
            this.mEndDelayed = delayed;
            this.mEndAction = runnable;
            return this;
        }

        protected void animateProperty(int constantName, float toValue) {
            float fromValue = getValue(constantName);
            setFromValue(constantName, fromValue);
            setDeltaValue(constantName, toValue - fromValue);
        }

        protected void animatePropertyBy(int constantName, float byValue) {
            setFromValue(constantName, getValue(constantName));
            setDeltaValue(constantName, byValue);
        }

        protected float getValue(int propertyConstant) {
            switch (propertyConstant) {
                case TRANSLATION_X:
                    return mView.getTranslationX();
                case TRANSLATION_Y:
                    return mView.getTranslationY();
                case TRANSLATION_Z:
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        return mView.getTranslationZ();
                    }
                    break;
                case ROTATION:
                    return mView.getRotation();
                case ROTATION_X:
                    return mView.getRotationX();
                case ROTATION_Y:
                    return mView.getRotationY();
                case SCALE_X:
                    return mView.getScaleX();
                case SCALE_Y:
                    return mView.getScaleY();
                case X:
                    return mView.getX();
                case Y:
                    return mView.getY();
                case Z:
                    if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                        return mView.getElevation() + mView.getTranslationZ();
                    }
                    break;
                case ALPHA:
                    return mView.getAlpha();
                case WIDTH:
                    return mView.getWidth();
                case HEIGHT:
                    return mView.getHeight();
                case SCROLL_X:
                    return mView.getScrollX();
                case SCROLL_Y:
                    return mView.getScrollY();
            }
            return 0;
        }

        protected void animation(float progress) {
            /**
             * TRANSLATION
             */
            if (hasAnimation(TRANSLATION_X)) {
                mView.setTranslationX(getFromValue(TRANSLATION_X) + getDeltaValue(TRANSLATION_X) * progress);
            }
            if (hasAnimation(TRANSLATION_Y)) {
                mView.setTranslationY(getFromValue(TRANSLATION_Y) + getDeltaValue(TRANSLATION_Y) * progress);
            }
            if (hasAnimation(TRANSLATION_Z) && Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                mView.setTranslationZ(getFromValue(TRANSLATION_Z) + getDeltaValue(TRANSLATION_Z) * progress);
            }
            /**
             * SCALE
             */
            if (hasAnimation(SCALE_X)) {
                mView.setScaleX(getFromValue(SCALE_X) + getDeltaValue(SCALE_X) * progress);
            }
            if (hasAnimation(SCALE_Y)) {
                mView.setScaleY(getFromValue(SCALE_Y) + getDeltaValue(SCALE_Y) * progress);
            }
            /**
             * ROTATION
             */
            if (hasAnimation(ROTATION)) {
                mView.setRotation(getFromValue(ROTATION) + getDeltaValue(ROTATION) * progress);
            }
            if (hasAnimation(ROTATION_X)) {
                mView.setRotationX(getFromValue(ROTATION_X) + getDeltaValue(ROTATION_X) * progress);
            }
            if (hasAnimation(ROTATION_Y)) {
                mView.setRotationY(getFromValue(ROTATION_Y) + getDeltaValue(ROTATION_Y) * progress);
            }
            /**
             * X&Y&Z
             */
            if (hasAnimation(X)) {
                mView.setX(getFromValue(X) + getDeltaValue(X) * progress);
            }
            if (hasAnimation(Y)) {
                mView.setY(getFromValue(Y) + getDeltaValue(Y) * progress);
            }
            if (hasAnimation(Y) && Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                mView.setZ(getFromValue(Y) + getDeltaValue(Y) * progress);
            }
            /**
             *
             */
            if (hasAnimation(ALPHA)) {
                mView.setAlpha(getFromValue(ALPHA) + getDeltaValue(ALPHA) * progress);
            }
            /**
             * WIDTH&HEIGHT
             */
            if (hasAnimation(WIDTH)) {
                mView.getLayoutParams().width = (int) (getFromValue(WIDTH) + getDeltaValue(WIDTH) * progress);
            }
            if (hasAnimation(HEIGHT)) {
                mView.getLayoutParams().height = (int) (getFromValue(HEIGHT) + getDeltaValue(HEIGHT) * progress);
            }
            /**
             * SCROLL
             */
            if (hasAnimation(SCROLL_X)) {
                mView.setScrollX((int) (getFromValue(SCROLL_X) + getDeltaValue(SCROLL_X) * progress));
            }
            if (hasAnimation(SCROLL_Y)) {
                mView.setScrollY((int) (getFromValue(SCROLL_Y) + getDeltaValue(SCROLL_Y) * progress));
            }
            mView.requestLayout();
        }

    }
}
